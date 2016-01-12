#include "stdafx.h"
#include "Session.h"

#ifdef ENABLE_IPV4
Session::Session(TcpServer &server, sockaddr_in addr, int fd)
	: server(server), fd(fd), sAddr(addr)//, writtenEvent(false)
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
	: server(server), IsIPv6(true), fd(fd), sAddr6(addr)//, writtenEvent(false)
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
	debug("fd = %u, entering read", (unsigned int)fd);
	while (IsAlive)
	{
		size_t currentLength;
		switch (readState)
		{
		case READSTATE_NONE:
			readLength = 0;
			readState = READSTATE_READING_LENGTH;
			continue;
			break;
		case READSTATE_READING_LENGTH:
			while (readLength < sizeof(package_len_t))
			{
				currentLength = bufferevent_read(buffev, lengthBuffer + readLength, sizeof(package_len_t) - readLength);
				if (currentLength <= 0) break;

				readLength += currentLength;
			}

			if (readLength == sizeof(package_len_t))
			{
				package_len_t dataLength = *(package_len_t *)lengthBuffer;
				dataLength = ntohpacklen(dataLength);

				if (lengthBuffer[0] == 'G' && lengthBuffer[1] == 'E' && lengthBuffer[2] == 'T' && lengthBuffer[3] == ' ')
				{
					// http request!
					lineBuffer = "GET ";
					readState = READSTATE_READING_LINE;
					continue;
				}
				else if (dataLength == 0 || dataLength > PACKAGE_MAXLENGTH)
				{
					// package is zero or too long
					debug("fd = %u, data length: %d == 0 || too long", (unsigned int)fd, dataLength);
					readState = READSTATE_ERROR;
					continue;
				}

				currentPackage = (Package*)malloc(sizeof(Package) + dataLength);
				if (!currentPackage)
				{
					throw bad_alloc();
				}
				currentPackage->length = dataLength;

				readLength = 0;
				readState = READSTATE_READING_DATA;
				continue;
			}
			break;
		case READSTATE_READING_DATA:
			debug("fd = %u, recv %d", (unsigned int)fd, currentPackage->length);
			while (readLength < currentPackage->length)
			{
				currentLength = bufferevent_read(buffev, currentPackage->data + readLength, currentPackage->length - readLength);
				if (currentLength <= 0) break;
				
				readLength += currentLength;
			}

			if (readLength == currentPackage->length)
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
		//case READSTATE_DROPPING_DATA:
		//	char buffer[16384];
		//	while (readLength < currentPackage->length)
		//	{
		//		currentLength = bufferevent_read(buffev, buffer, sizeof(buffer));
		//		if (currentLength <= 0) break;

		//		readLength += currentLength;
		//	}

		//	if (readLength == currentPackage->length)
		//	{
		//		OnPackage(currentPackage); // TODO: sync or async?
		//		if (currentPackage)
		//		{
		//			delete currentPackage;
		//			currentPackage = NULL;
		//		}
		//		readLength = 0;
		//		readState = READSTATE_NONE;
		//		continue;
		//	}
		//	break;
		case READSTATE_READING_LINE:
			char ch;
			if (bufferevent_read(buffev, &ch, 1) > 0)
			{
				lineBuffer += ch;
				if (lineBuffer.length() > 16384)
				{
					// header too long
					readState = READSTATE_ERROR;
					continue;
				}
				if (endwith(lineBuffer, "\r\n\r\n"))
				{
					Package *pack = MakePackage(lineBuffer);
					OnPackage(pack);
					readState = READSTATE_NONE;
					continue;
				}
				continue;
			}
			break;
		case READSTATE_ERROR:
			SendPackage("{\"error\": \"unknown error\"}");
			CloseOnWritten = true;
			break;
		}
		break;
	}
	debug("fd = %u, exitting read", (unsigned int)fd);
}

void Session::WriteCallback()
{
	if (!IsAlive) return;

	{
		unique_lock<mutex> lck(writeLock);
		if (!writeBufferHasData)
		{
			debug("fd = %u, dropping first write callback calling", (unsigned int)fd);
			// drop first write callback calling
			writeBufferHasData = true;
			return;
		}
	}

	debug("fd = %u, write callback called", (unsigned int)fd);

	if (CloseOnWritten)
	{
		debug("fd = %u, CloseOnWritten is set, closing", (unsigned int)fd);
		Close();
	}
}

void Session::ErrorCallback(short event)
{
	if (!IsAlive) return;

	const char *msg = NULL;
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

	debug("fd = %u, %s", (unsigned int)fd, msg);
	
	Close();
}

void Session::DoQueue()
{
	if (!IsAlive) return;

	unique_lock<mutex> lck(writeLock);
	debug("%d, dequeuing", fd);

	while (!pendingPackages.empty())
	{
		Package *&pack = pendingPackages.front();
		if (pack->length > 0)
		{
			size_t toSendLength = sizeof(Package) + ntohpacklen(pack->length);
			char *packdata = (char*)pack;

			bufferevent_write(buffev, packdata, toSendLength);
		}
		delete pack;
		pack = NULL;

		pendingPackages.pop();
	}

	writeBufferHasData = true;

	debug("%d, dequeued", fd);
}

void Session::OnPackage(Package *&pack)
{
	if (!pack || pack->length == 0) return;
	pack->data[pack->length - 1] = '\0';
	debug("on package (fd = %u): %s", (unsigned int)fd, pack->data);

	// TODO: process received package
	{
		string str = pack->data;
		if (str.substr(0, 4) == "GET ")
		{
			unique_lock<mutex> lck(writeLock);

			string content = "<h1>It really works!</h1><p>" + to_string(rand()) + "</p>";
			string header = "HTTP/1.0 200 OK\r\nServer: Wandai\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: " + to_string(content.length()) + "\r\n\r\n";
			string data = header + content;

			//writeBufferHasData = true;
			bufferevent_write(buffev, data.c_str(), data.length());
			CloseOnWritten = true;
		}
	}
	
	delete pack;
	pack = NULL;
}

void Session::SendPackage(Package *&pack)
{
	if (!pack || pack->length == 0) return;
	pack->length = htonpacklen(pack->length);
	//writtenEvent.Reset();
	pendingPackages.push(pack);

	DoQueue();
}

void Session::SendPackage(string str)
{
	Package *pack = MakePackage(str);
	SendPackage(pack);
}

void Session::Close()
{
	if (!IsAlive) return;
	IsAlive = false;

	debug("fd: %d, closing", fd);
	
	if (buffev)
	{
		ClearCallbacks(); // need clear callbacks BEFORE shutdown.
	}
	
	//writtenEvent.Set();
	
	if (fd)
	{
		shutdown(fd, SHUT_RDWR);
		close(fd);
		fd = NULL;
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
	package_len_t len = (package_len_t)((strdata.length() + 1) * sizeof(char));
	Package *pack = (Package*)malloc(sizeof(Package) + len);
	pack->length = len;
	memcpy(pack->data, strdata.c_str(), len);
	return pack;
}

package_len_t htonpacklen(package_len_t len)
{
	if (sizeof(package_len_t) == 2)
	{
		return (package_len_t)htons(len);
	}
	else if (sizeof(package_len_t) == 4)
	{
		return (package_len_t)htonl(len);
	}
}

package_len_t ntohpacklen(package_len_t len)
{
	if (sizeof(package_len_t) == 2)
	{
		return (package_len_t)ntohs(len);
	}
	else if (sizeof(package_len_t) == 4)
	{
		return (package_len_t)ntohl(len);
	}
}

void readCallbackDispatcher(bufferevent *buffev, void *arg)
{
	((Session*)arg)->ReadCallback();
	((Session*)arg)->server.CleanSession((Session*)arg);
}

void writeCallbackDispatcher(bufferevent *buffev, void *arg)
{
	((Session*)arg)->WriteCallback();
	((Session*)arg)->server.CleanSession((Session*)arg);
}

void errorCallbackDispatcher(bufferevent *buffev, short event, void *arg)
{
	((Session*)arg)->ErrorCallback(event);
	((Session*)arg)->server.CleanSession((Session*)arg);
}
