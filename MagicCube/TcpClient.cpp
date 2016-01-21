#include "stdafx.h"
#include "TcpClient.h"

#ifndef NONET

TcpClient::TcpClient()
	: doQueueEvent(false, true)
{
}


TcpClient::~TcpClient()
{
	Close();
	queueLock.lock();
	queueLock.unlock();
}

bool TcpClient::Connect(const string &address, short port)
{
#ifdef ENABLE_IPV6
	if (address.find(':') != string::npos) // ipv6
	{
		fd = socket(AF_INET6, SOCK_STREAM, 0);
		if (fd <= 0)
		{
			fd = static_cast<evutil_socket_t>(0);
			_perror("socket6");
			return false;
		}

		sockaddr_in6 sin6;
		sin6.sin6_family = AF_INET6;
		evutil_inet_pton(AF_INET6, address.c_str(), &(sin6.sin6_addr.s6_addr));
		sin6.sin6_port = htons(port);

		if (connect(fd, reinterpret_cast<const sockaddr*>(&sin6), sizeof(sin6)) != 0)
		{
			_perror("connect6");
#ifdef _WIN32
			closesocket(fd);
#else
			close(fd);
#endif
			fd = static_cast<evutil_socket_t>(0);
			return false;
		}

		return true;
	}
#else
	if (address.find(':') != string::npos) // ipv6
	{
		log_warn("IPv6 is not supported.");
		return false;
	}
#endif

#ifdef ENABLE_IPV4
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd <= 0)
	{
		fd = static_cast<evutil_socket_t>(0);
		_perror("socket");
		return false;
	}

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	evutil_inet_pton(AF_INET, address.c_str(), &(sin.sin_addr.s_addr));
	sin.sin_port = htons(port);

	if (connect(fd, reinterpret_cast<const sockaddr*>(&sin), sizeof(sin)) != 0)
	{
		_perror("connect");
#ifdef _WIN32
		closesocket(fd);
#else
		close(fd);
#endif
		fd = static_cast<evutil_socket_t>(0);
		return false;
	}

	return true;
#endif
}

void TcpClient::Close()
{
	IsAlive = false;

	if (fd)
	{
#ifdef _WIN32
		closesocket(fd);
#else
		close(fd);
#endif
		fd = static_cast<evutil_socket_t>(0);
	}

	if (threadReader)
	{
		if (threadReader->native_handle())
		{
			threadReader->join();
		}
		delete threadReader;
		threadReader = NULL;
	}

	doQueueEvent.Set();

	if (threadWriter)
	{
		if (threadWriter->native_handle())
		{
			threadWriter->join();
		}
		delete threadWriter;
		threadWriter = NULL;
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

void TcpClient::Wait()
{
	if (threadWriter)
	{
		threadWriter->join();
	}
}

void TcpClient::Start()
{
	if (IsAlive) return;
	IsAlive = true;

	doQueueEvent.Reset();

	threadReader = new thread([this] {this->Reader(); });
	threadWriter = new thread([this] {this->Writer(); });
}

void TcpClient::Reader()
{
	while (IsAlive)
	{
		const size_t headerLength = sizeof(PackageHeader);

		// read header
		char headerBuffer[headerLength];
		if (!bufferedRecv(headerBuffer, headerLength))
		{
			log_debug("Read error, disconnected.");
			break;
		}

		PackageHeader header = *reinterpret_cast<PackageHeader*>(headerBuffer);
		header.DataLength = ntohpacklen(header.DataLength);

		if (memcmp(headerBuffer, MAGIC_MARK, min(headerLength, sizeof(MAGIC_MARK) - 1)) != 0)
		{
			log_debug("Protocol mismatch.");
			break;
		}

		if (header.DataLength == 0 || header.DataLength > PACKAGE_MAXLENGTH)
		{
			log_debug("Package is empty or too long.");
			break;
		}

		Package *pack = new (header.DataLength) Package;
		if (!pack)
		{
			throw bad_alloc();
		}
		pack->Header = header; // copy

		memset(pack->Data, 0x00, header.DataLength); // ensure

		if (!bufferedRecv(pack->Data, header.DataLength))
		{
			log_debug("Read error, disconnected.");
			break;
		}

		OnPackage(pack);
		if (pack)
		{
			delete pack;
			pack = NULL;
		}
	}

	// error or disconnected.
	IsAlive = false;
	doQueueEvent.Set();
}

void TcpClient::Writer()
{
	while (IsAlive)
	{

		doQueueEvent.Wait();

		{
			unique_lock<mutex> lckQueue(queueLock);

			while (!pendingPackages.empty())
			{
				Package *&pack = pendingPackages.front();

				size_t dataLength = pack->Header.DataLength;
				pack->Header.DataLength = htonpacklen(pack->Header.DataLength);

				if (!bufferedSend(reinterpret_cast<char*>(pack), sizeof(PackageHeader) + dataLength))
				{
					log_debug("Send error, disconnected.");
					break;
				}

				if (pack)
				{
					delete pack;
					pack = NULL;
				}
				pendingPackages.pop();
			}
		}
	}

	// error or disconnected.
	IsAlive = false;
	doQueueEvent.Set();
}

void TcpClient::OnPackage(Package *&pack)
{
	if (!pack || pack->Header.DataLength == 0) return;
	pack->Data[pack->Header.DataLength - 1] = '\0';
	log_debug("on package (fd = %u): %s\n", static_cast<unsigned int>(fd), pack->Data);

	// TODO: process received package

	delete pack;
	pack = NULL;
}

void TcpClient::SendPackage(Package *&pack)
{
	if (!pack || pack->Header.DataLength == 0) return;

	{
		unique_lock<mutex> lckQueue(queueLock);
		pendingPackages.push(pack);
	}

	doQueueEvent.Set();
}

void TcpClient::SendPackage(string str)
{
	Package *pack = MakePackage(str);
	SendPackage(pack);
}

Package *TcpClient::MakePackage(string &strdata)
{
	package_len_t len = static_cast<package_len_t>((strdata.length() + 1) * sizeof(char));
	Package *pack = new (len) Package;
	memcpy(pack->Header.Magic, MAGIC_MARK, sizeof(MAGIC_MARK) - 1);
	pack->Header.DataLength = len;
	memcpy(pack->Data, strdata.c_str(), len);
	return pack;
}

bool TcpClient::bufferedRecv(char *buffer, size_t size)
{
	size_t readLength = 0;

	while (IsAlive && readLength < size)
	{
		size_t wantRead = min(BUFFER_SIZE, size - readLength);
		size_t currentRead = recv(fd, buffer + readLength, wantRead, 0);
		if (currentRead <= 0) // error or disconnected
		{
			break;
		}
		readLength += currentRead;
	}

	return readLength == size;
}

bool TcpClient::bufferedSend(char *buffer, size_t size)
{
	size_t sentLength = 0;

	while (IsAlive && sentLength < size)
	{
		size_t wantSend = min(BUFFER_SIZE, size - sentLength);
		size_t currentSend = send(fd, buffer + sentLength, wantSend, 0);
		if (currentSend <= 0) // error or disconnected
		{
			break;
		}
		sentLength += currentSend;
	}

	return sentLength == size;
}

#endif