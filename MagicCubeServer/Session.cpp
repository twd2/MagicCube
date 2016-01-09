#include "stdafx.h"
#include "Session.h"

#ifdef ENABLE_IPV4
Session::Session(event_base *evbase, sockaddr_in addr, int fd) : evbase(evbase), fd(fd), sAddr(addr)
{
	char buffer[INET6_ADDRSTRLEN + 1] = { 0 };
	evutil_inet_ntop(addr.sin_family, &(addr.sin_addr), buffer, sizeof(buffer));
	RemoteAddress = buffer;
	RemotePort = ntohs(addr.sin_port);

	buffev = bufferevent_socket_new(evbase, fd, BEV_OPT_CLOSE_ON_FREE);
}
#endif

#ifdef ENABLE_IPV6
Session::Session(event_base *evbase, sockaddr_in6 addr, int fd) : evbase(evbase), IsIPv6(true), fd(fd), sAddr6(addr)
{
	char buffer[INET6_ADDRSTRLEN + 1] = { 0 };
	evutil_inet_ntop(addr.sin6_family, &(addr.sin6_addr), buffer, sizeof(buffer));
	RemoteAddress = buffer;
	RemotePort = ntohs(addr.sin6_port);

	buffev = bufferevent_socket_new(evbase, fd, BEV_OPT_CLOSE_ON_FREE);
}
#endif

Session::~Session()
{
	shutdown(fd, SHUT_RDWR);
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
	Alive = false;
}

void Session::SetCallbacks()
{
	bufferevent_setcb(buffev, 
		readCallbackDispatcher, 
		writeCallbackDispatcher,
		errorCallbackDispatcher, this);
	bufferevent_enable(buffev, EV_READ | EV_WRITE | EV_PERSIST);

	SendPackage(string("{\"type\": \"hello\", }"));
}

void Session::ReadCallback()
{
	printf("entering read\n");
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

				if (dataLength > PACKAGE_MAXLENGTH)
				{
					// package is too long
					printf("%d, too long\n", fd);
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
			this->~Session();
			break;
		}
		break;
	}
	printf("exitting read\n");
}

void Session::WriteCallback()
{
	printf("entering write\n");
	while (Alive)
	{
		switch (writeState)
		{
		case WRITESTATE_NONE:
			if (!pendingPackages.empty())
			{
				writtenLength = 0;
				writeState = WRITESTATE_WRITING;
				continue;
			}
			break;
		case WRITESTATE_WRITING:
		{
			Package *pack = pendingPackages.front();
			size_t toSendLength = sizeof(Package) + ntohpacklen(pack->length);
			char *packdata = (char *)pack;

			if (writtenLength == toSendLength)
			{
				delete pack;
				pendingPackages.pop();
				writeState = WRITESTATE_NONE;
				continue;
			}
			else
			{
				size_t currentLength = min(toSendLength - writtenLength, (size_t)16384);
				bufferevent_write(buffev, packdata + writtenLength, currentLength);
				writtenLength += currentLength;
			}
		}
			break;
		case WRITESTATE_ERROR:
			this->~Session();
			break;
		}
		break;
	}
	printf("exitting write\n");
}

void Session::ErrorCallback(short event)
{
	//printf("fd = %u, ", fd);
	if (event & BEV_EVENT_TIMEOUT)
	{
		//printf("timed out\n"); // if bufferevent_set_timeouts() called
	}
	else if (event & BEV_EVENT_EOF)
	{
		//printf("connection closed\n");
	}
	else if (event & BEV_EVENT_ERROR)
	{
		//printf("some other error\n");
	}
	
	this->~Session();
}

void Session::OnPackage(Package *&pack)
{
	if (!pack) return;
	pack->data[pack->length - 1] = 0;
	printf("on package %d: %s\n", fd, pack->data);

	delete pack;
	pack = NULL;
}

void Session::SendPackage(Package *&pack)
{
	if (!pack) return;
	pack->length = htonpacklen(pack->length);
	pendingPackages.push(pack);
	WriteCallback();
}

void Session::SendPackage(string str)
{
	Package *pack = MakePackage(str);
	SendPackage(pack);
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
	((Session*)arg)->ErrorCallback(event);
}
