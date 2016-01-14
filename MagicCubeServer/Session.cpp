#include "stdafx.h"
#include "Session.h"

map<ReadErrorType, string> ReadErrorMessage = 
{
	{ READERROR_PROTOCOL_MISMATCH,  "protocol mismatch"   },
	{ READERROR_PACKAGE_TOO_LONG,   "package is too long" },
	{ READERROR_PACKAGE_EMPTY,      "package is empty"    },
	{ READERROR_UNKNOWN,            "unknown"             }
};

#ifdef ENABLE_IPV4
Session::Session(TcpServer &server, sockaddr_in addr, int fd)
	: server(server), fd(fd), sAddr(addr)
{
	char buffer[INET6_ADDRSTRLEN + 1] = { 0 };
	evutil_inet_ntop(addr.sin_family, &(addr.sin_addr), buffer, sizeof(buffer));
	RemoteAddress = buffer;
	RemotePort = ntohs(addr.sin_port);

	evutil_make_socket_nonblocking(fd);
	buffev = bufferevent_socket_new(server.Base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE | BEV_OPT_DEFER_CALLBACKS);
}
#endif

#ifdef ENABLE_IPV6
Session::Session(TcpServer &server, sockaddr_in6 addr, int fd)
	: IsIPv6(true), server(server), fd(fd), sAddr6(addr)
{
	char buffer[INET6_ADDRSTRLEN + 1] = { 0 };
	evutil_inet_ntop(addr.sin6_family, &(addr.sin6_addr), buffer, sizeof(buffer));
	RemoteAddress = buffer;
	RemotePort = ntohs(addr.sin6_port);

	evutil_make_socket_nonblocking(fd);
	buffev = bufferevent_socket_new(server.Base, fd, BEV_OPT_CLOSE_ON_FREE);
}
#endif

Session::~Session()
{
	Close();
	readLock.lock();
	readLock.unlock();
	writeLock.lock();
	writeLock.unlock();
}

void Session::KeepAlive()
{
	LastAlive = time(NULL);
}

void Session::SetCallbacks()
{
	SetCallbacks(true, true, true);
}

void Session::SetCallbacks(bool read, bool write, bool event)
{
	bufferevent_data_cb readCB = NULL, writeCB = NULL;
	bufferevent_event_cb eventCB = NULL;
	short eventsEN = EV_PERSIST;
	if (read)
	{
		readCB = readCallbackDispatcher;
		eventsEN |= EV_READ;
	}
	if (write)
	{
		writeCB = writeCallbackDispatcher;
		eventsEN |= EV_WRITE;
	}
	if (event)
	{
		eventCB = errorCallbackDispatcher;
	}

	bufferevent_setcb(buffev, 
		readCB,
		writeCB,
		eventCB, (void*)this);

	if (read || write || event)
	{
		bufferevent_enable(buffev, eventsEN);
	}
	else
	{
		bufferevent_disable(buffev, EV_READ | EV_WRITE);
	}
}

void Session::ClearCallbacks()
{
	SetCallbacks(false, false, false);
}

void Session::ReadCallback()
{
	if (!IsAlive) return;

	unique_lock<mutex> lck(readLock);
	debug("fd = %u, entering read", static_cast<unsigned int>(fd));
	while (IsAlive)
	{
		KeepAlive();
		switch (readState)
		{
		case READSTATE_NONE:
			readLength = 0;
			readState = READSTATE_READING_HEADER;
			continue;
			break;
		case READSTATE_READING_HEADER:
		{
			const size_t headerLength = sizeof(PackageHeader);

			while (readLength < headerLength)
			{
				size_t currentLength = bufferevent_read(buffev, headerBuffer + readLength, headerLength - readLength);
				if (currentLength <= 0) break;

				readLength += currentLength;
			}

			if (readLength == headerLength)
			{
				PackageHeader header = *reinterpret_cast<PackageHeader*>(headerBuffer);
				header.DataLength = ntohpacklen(header.DataLength);

				if (memcmp(headerBuffer, "GET ", min(headerLength, (size_t)4)) == 0)
				{
					// http request!
					char buff[sizeof(PackageHeader) + 1];
					memcpy(buff, headerBuffer, sizeof(PackageHeader));
					buff[sizeof(PackageHeader)] = '\0';

					lineBuffer = buff;

					readState = READSTATE_READING_LINE;
					continue;
				}
				else if (memcmp(headerBuffer, MAGIC_MARK, min(headerLength, sizeof(MAGIC_MARK) - 1)) != 0)
				{
					readErrorCode = READERROR_PROTOCOL_MISMATCH;
					readState = READSTATE_ERROR;
					continue;
				}
				
				if (header.DataLength == 0 || header.DataLength > PACKAGE_MAXLENGTH)
				{
					// package is zero or too long
					debug("fd = %u, data length: %d == 0 || too long", static_cast<unsigned int>(fd), header.DataLength);
					
					if (header.DataLength == 0)
						readErrorCode = READERROR_PACKAGE_EMPTY;
					else
						readErrorCode = READERROR_PACKAGE_TOO_LONG;
					
					readState = READSTATE_ERROR;
					continue;
				}

				currentPackage = new (header.DataLength) Package;
				if (!currentPackage)
				{
					throw bad_alloc();
				}
				currentPackage->Header = header; // copy

				memset(currentPackage->Data, 0x00, header.DataLength); // ensure

				readLength = 0;
				readState = READSTATE_READING_DATA;
				continue;
			}
			break;
		}
		case READSTATE_READING_DATA:
		{
			while (readLength < currentPackage->Header.DataLength)
			{
				size_t currentLength = bufferevent_read(buffev, currentPackage->Data + readLength, currentPackage->Header.DataLength - readLength);
				debug("fd = %u, recv %u", static_cast<unsigned int>(fd), currentLength);
				if (currentLength <= 0) break;

				readLength += currentLength;
			}

			if (readLength == currentPackage->Header.DataLength)
			{
				OnPackage(currentPackage); // TODO: sync or async?
				if (currentPackage)
				{
					delete currentPackage;
					currentPackage = NULL;
				}
				readLength = 0;
				readState = READSTATE_NONE;
				continue;
			}
			break;
		}
		case READSTATE_READING_LINE:
		{
			char ch;
			if (bufferevent_read(buffev, &ch, 1) > 0)
			{
				lineBuffer += ch;
				if (lineBuffer.length() > HTTP_HEADER_MAXLENGTH)
				{
					// http header is too long
					readErrorCode = READERROR_PACKAGE_TOO_LONG;
					readState = READSTATE_ERROR;
					continue;
				}
				if (endsWith(lineBuffer, "\r\n\r\n"))
				{
					OnHTTPRequest(lineBuffer);
					readState = READSTATE_NONE;
					continue;
				}
				continue;
			}
			break;
		}
		case READSTATE_ERROR:
		{
			SendPackage("{\"error\": \"" + ReadErrorMessage[readErrorCode] + "\"}");
			FlushAndClose();
			readErrorCode = READERROR_UNKNOWN;
			break;
		}
		}
		break;
	}
	debug("fd = %u, exitting read", static_cast<unsigned int>(fd));
}

void Session::WriteCallback()
{
	if (!IsAlive) return;

	{
		unique_lock<mutex> lck(writeLock);
		if (isFirstCall)
		{
			debug("fd = %u, dropping first write callback calling", static_cast<unsigned int>(fd));
			// drop first write callback calling
			isFirstCall = false;
			return;
		}
	}

	debug("fd = %u, write callback called", static_cast<unsigned int>(fd));

	if (closeAfterWritten)
	{
		debug("fd = %u, closeAfterWritten is set, closing", static_cast<unsigned int>(fd));
		Close();
	}
}

void Session::ErrorCallback(short event)
{
	if (!IsAlive) return;

	const char *msg = "";
	if (event & BEV_EVENT_TIMEOUT)
	{
		msg = "timed out"; // if bufferevent_set_timeouts() called
	}
	else if (event & BEV_EVENT_EOF)
	{
		msg = "connection closed";
	}
	else if (event & BEV_EVENT_ERROR)
	{
		msg = "some other error";
		__perror("error");
	}

	debug("fd = %u, %s", static_cast<unsigned int>(fd), msg);
	
	Close();
}

void Session::DoQueue()
{
	if (!IsAlive) return;

	unique_lock<mutex> lck(writeLock);
	debug("fd = %u, dequeuing", static_cast<unsigned int>(fd));

	while (!pendingPackages.empty())
	{
		Package *&pack = pendingPackages.front();
		package_len_t packLen = ntohpacklen(pack->Header.DataLength);
		if (packLen > 0)
		{
			size_t toSendLength = sizeof(PackageHeader) + packLen;
			char *packData = reinterpret_cast<char*>(pack);

			bufferevent_write(buffev, packData, toSendLength);
		}
		delete pack;
		pack = NULL;

		pendingPackages.pop();
	}

	debug("fd = %u, dequeued", static_cast<unsigned int>(fd));
}

void Session::OnHTTPRequest(const string &req)
{
	debug("regarded as HTTP request (fd = %u)", static_cast<unsigned int>(fd));

	unique_lock<mutex> lck(writeLock);

	string content = "<h1>It really works!</h1><p>" /*+ to_string(rand()) +*/ "</p>";
	string header = "HTTP/1.0 200 OK\r\nServer: Wandai/0.1\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: " + to_string(content.length()) + "\r\n\r\n";
	string data = header + content;

	bufferevent_write(buffev, data.c_str(), data.length());
	FlushAndClose();
}

void Session::OnPackage(Package *&pack)
{
	if (!pack || pack->Header.DataLength == 0) return;
	pack->Data[pack->Header.DataLength - 1] = '\0';
	debug("on package (fd = %u): %s", static_cast<unsigned int>(fd), pack->Data);

	// TODO: process received package
	
	delete pack;
	pack = NULL;
}

void Session::SendPackage(Package *&pack)
{
	if (!pack || pack->Header.DataLength == 0) return;
	pack->Header.DataLength = htonpacklen(pack->Header.DataLength);
	pendingPackages.push(pack);

	DoQueue();
}

void Session::SendPackage(string str)
{
	Package *pack = MakePackage(str);
	SendPackage(pack);
}

void Session::FlushAndClose()
{
	closeAfterWritten = true;
}

void Session::Close()
{
	if (!IsAlive) return;
	IsAlive = false;

	debug("fd = %u, closing", static_cast<unsigned int>(fd));
	
	if (buffev)
	{
		ClearCallbacks(); // need clear callbacks BEFORE shutdown.
	}
	
	if (fd)
	{
		shutdown(fd, SHUT_RDWR);
#ifdef _WIN32
		closesocket(fd);
#else
		close(fd);
#endif
		fd = static_cast<evutil_socket_t>(0);
	}
	
	if (buffev)
	{
		bufferevent_free(buffev);
		buffev = NULL;
	}
	
	if (currentPackage)
	{
		delete currentPackage;
		currentPackage = NULL;
	}
	
	while (!pendingPackages.empty())
	{
		Package *&pack = pendingPackages.front();
		if (pack)
		{
			delete pack;
			pack = NULL;
		}
		pendingPackages.pop();
	}
}

Package *Session::MakePackage(string &strdata)
{
	package_len_t len = static_cast<package_len_t>((strdata.length() + 1) * sizeof(char));
	Package *pack = new (len) Package;
	memcpy(pack->Header.Magic, MAGIC_MARK, sizeof(MAGIC_MARK) - 1);
	pack->Header.DataLength = len;
	memcpy(pack->Data, strdata.c_str(), len);
	return pack;
}

package_len_t htonpacklen(package_len_t len)
{
	if (sizeof(package_len_t) == 2)
	{
		return static_cast<package_len_t>(htons(len));
	}
	else if (sizeof(package_len_t) == 4)
	{
		return static_cast<package_len_t>(htonl(len));
	}
}

package_len_t ntohpacklen(package_len_t len)
{
	if (sizeof(package_len_t) == 2)
	{
		return static_cast<package_len_t>(ntohs(len));
	}
	else if (sizeof(package_len_t) == 4)
	{
		return static_cast<package_len_t>(ntohl(len));
	}
}

void readCallbackDispatcher(bufferevent *buffev, void *arg)
{
	Session *sess = reinterpret_cast<Session*>(arg);

	sess->ReadCallback();
	sess->server.CleanSession((Session*)arg);
}

void writeCallbackDispatcher(bufferevent *buffev, void *arg)
{
	Session *sess = reinterpret_cast<Session*>(arg);

	sess->WriteCallback();
	sess->server.CleanSession(sess);
}

void errorCallbackDispatcher(bufferevent *buffev, short event, void *arg)
{
	Session *sess = reinterpret_cast<Session*>(arg);

	sess->ErrorCallback(event);
	sess->server.CleanSession(sess);
}
