#include "stdafx.h"
#include "Session.h"

#ifdef ENABLE_IPV4
Session::Session(event_base *evbase, sockaddr_in addr, int fd) 
	: evbase(evbase), fd(fd), sAddr(addr), writtenEvent(false)
{
	char buffer[INET6_ADDRSTRLEN + 1] = { 0 };
	evutil_inet_ntop(addr.sin_family, &(addr.sin_addr), buffer, sizeof(buffer));
	RemoteAddress = buffer;
	RemotePort = ntohs(addr.sin_port);

	evutil_make_socket_nonblocking(fd);

	buffev = bufferevent_socket_new(evbase, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE | BEV_OPT_DEFER_CALLBACKS);
	printf("buffev: %p\n", buffev);
}
#endif

#ifdef ENABLE_IPV6
Session::Session(event_base *evbase, sockaddr_in6 addr, int fd)
	: evbase(evbase), IsIPv6(true), fd(fd), sAddr6(addr), writtenEvent(false)
{
	char buffer[INET6_ADDRSTRLEN + 1] = { 0 };
	evutil_inet_ntop(addr.sin6_family, &(addr.sin6_addr), buffer, sizeof(buffer));
	RemoteAddress = buffer;
	RemotePort = ntohs(addr.sin6_port);

	evutil_make_socket_nonblocking(fd);
	buffev = bufferevent_socket_new(evbase, fd, BEV_OPT_CLOSE_ON_FREE);
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
		eventCB, this);

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
	if (!Alive) return;
	unique_lock<mutex> lck(readLock);
	printf("%d, entering read\n", fd);
	while (Alive)
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
			while (readLength < sizeof(package_len_t) &&
				(currentLength =
					bufferevent_read(
						buffev,
						lengthBuffer + readLength,
						sizeof(package_len_t) - readLength
						)
					) > 0)
			{
				readLength += currentLength;
			}

			if (readLength == sizeof(package_len_t))
			{
				package_len_t dataLength = *(package_len_t *)lengthBuffer;
				dataLength = ntohpacklen(dataLength);

				if (dataLength == 0 || dataLength > PACKAGE_MAXLENGTH)
				{
					// package is zero or too long
					printf("%d, == 0 || too long\n", fd);
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
			printf("recv %d\n", currentPackage->length);
			while (readLength < currentPackage->length &&
				(currentLength =
					bufferevent_read(
						buffev,
						currentPackage->data + readLength,
						currentPackage->length - readLength
						)
					) > 0)
			{
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
		case READSTATE_ERROR:
			SendPackage("{\"error\": \"unknown error\"}");
			Close();
			break;
		}
		break;
	}
	printf("%d, exitting read\n", fd);
}

void Session::WriteCallback()
{
	if (!Alive) return;
	printf("%d, write cb\n", fd);
}

void Session::ErrorCallback(short event)
{
	if (!Alive) return;
	printf("fd = %u, ", fd);
	if (event & BEV_EVENT_TIMEOUT)
	{
		printf("timed out\n"); // if bufferevent_set_timeouts() called
	}
	else if (event & BEV_EVENT_EOF)
	{
		printf("connection closed\n");
	}
	else if (event & BEV_EVENT_ERROR)
	{
		printf("some other error\n");
		_perror("error");
	}
	
	Close();
}

void Session::DoQueue()
{
	if (!Alive) return;
	unique_lock<mutex> lck(writeLock);
	printf("%d, entering write\n", fd);
	while (!pendingPackages.empty())
	{
		Package *&pack = pendingPackages.front();
		size_t toSendLength = sizeof(Package) + ntohpacklen(pack->length);
		char *packdata = (char *)pack;

		bufferevent_write(buffev, packdata, toSendLength);
		delete pack;
		pack = NULL;

		pendingPackages.pop();
	}
	/*while (Alive)
	{
		switch (writeState)
		{
		case WRITESTATE_NONE:
			if (!pendingPackages.empty())
			{
				writtenLength = 0;
				writeState = WRITESTATE_WRITING;
				writtenEvent.Reset();
				printf("%d, changing state to WRITING\n", fd);
				continue;
			}
			else
			{
				writtenEvent.Set();
			}
			break;
		case WRITESTATE_WRITING:
		{
			Package *&pack = pendingPackages.front();
			size_t toSendLength = sizeof(Package) + ntohpacklen(pack->length);
			char *packdata = (char *)pack;

			if (writtenLength == toSendLength)
			{
				delete pack;
				pack = NULL;
				pendingPackages.pop();
				writeState = WRITESTATE_NONE;
				printf("%d, changing state to NONE\n", fd);
				continue;
			}
			else
			{
				printf("%d, writing\n", fd);
				size_t currentLength = toSendLength - writtenLength;//min(toSendLength - writtenLength, (size_t)2);
				bufferevent_write(buffev, packdata + writtenLength, currentLength);
				writtenLength += currentLength;
				printf("%d, wrote %u/%u\n", fd, writtenLength, toSendLength);
			}
		}
		break;
		case WRITESTATE_ERROR:
			Close();
			break;
		}
		break;
	}*/
	printf("%d, exitting write\n", fd);
}

void Session::OnPackage(Package *&pack)
{
	if (!pack) return;
	pack->data[pack->length - 1] = 0;
	printf("on package %d: %s\n", fd, pack->data);
	// TODO: process received package
	delete pack;
	pack = NULL;
}

void Session::SendPackage(Package *&pack)
{
	if (!pack) return;
	pack->length = htonpacklen(pack->length);
	writtenEvent.Reset();
	pendingPackages.push(pack);
	DoQueue();
}

void Session::SendPackage(string str)
{
	Package *pack = MakePackage(str);
	SendPackage(pack);
}

void Session::FlushQueue()
{
	while (!pendingPackages.empty())
	{
		printf("fd: %d, FlushQueue waiting\n", fd);
		DoQueue();
		writtenEvent.Wait();
	}
}

void Session::Close()
{
	if (!Alive) return;
	Alive = false;
	printf("fd: %d, closing\n", fd);
	if (buffev)
	{
		ClearCallbacks(); // need clear callbacks BEFORE shutdown.
	}
	writtenEvent.Set();
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
	package_len_t len = (strdata.length() + 1) * sizeof(char);
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
}

void writeCallbackDispatcher(bufferevent *buffev, void *arg)
{
	((Session*)arg)->WriteCallback();
}

void errorCallbackDispatcher(bufferevent *buffev, short event, void *arg)
{
	printf("error, arg=%p\n", arg);
	((Session*)arg)->ErrorCallback(event);
}
