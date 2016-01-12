#pragma once

#include <queue>
#include "Package.h"
#include "ManualEvent.h"

class TcpServer;

enum ReadStateType
{
	READSTATE_NONE,
	READSTATE_READING_HEADER,
	READSTATE_READING_DATA,
	READSTATE_DROPPING_DATA,
	READSTATE_READING_LINE,
	READSTATE_ERROR
};

enum ReadErrorType
{
	READERROR_PROTOCOL_MISMATCH,
	READERROR_PACKAGE_EMPTY,
	READERROR_PACKAGE_TOO_LONG,
	READERROR_UNKNOWN = -1
};
extern map<ReadErrorType, string> ReadErrorMessage;

class Session
{
public:
	bool IsAlive = true;
	bool IsIPv6 = false;
	time_t LastAlive = time(NULL);

	string RemoteAddress;
	unsigned short RemotePort;

	TcpServer &server;

#ifdef ENABLE_IPV4
	Session(TcpServer&, sockaddr_in, int);
#endif
#ifdef ENABLE_IPV6
	Session(TcpServer&, sockaddr_in6, int);
#endif
	~Session();

	// invoked again and again when doing complex calculation to avoid being cleaned
	void KeepAlive();
	void SetCallbacks();
	void SetCallbacks(bool, bool, bool);
	void ClearCallbacks();
	void ReadCallback();
	void WriteCallback();
	void ErrorCallback(short);
	void DoQueue();
	void OnPackage(Package*&);
	void SendPackage(Package*&);
	void SendPackage(string);

	void FlushAndClose();
	void Close();

	static Package *MakePackage(string&);

private:

	// prevent copying
	DISALLOW_COPY_AND_ASSIGN(Session);

	bufferevent *buffev;
	evutil_socket_t fd;

#ifdef ENABLE_IPV4
	sockaddr_in sAddr;
#endif

#ifdef ENABLE_IPV6
	sockaddr_in6 sAddr6;
#endif

	mutex readLock;
	ReadStateType readState = READSTATE_NONE;
	ReadErrorType readErrorCode = READERROR_UNKNOWN;
	size_t readLength;
	char headerBuffer[sizeof(PackageHeader)];
	string lineBuffer;
	Package *currentPackage = NULL;

	bool isFirstCall = true;
	queue<Package*> pendingPackages;
	mutex writeLock;
	bool closeAfterWritten = false;
};

package_len_t htonpacklen(package_len_t);
package_len_t ntohpacklen(package_len_t);
void readCallbackDispatcher(bufferevent*, void*);
void writeCallbackDispatcher(bufferevent*, void*);
void errorCallbackDispatcher(bufferevent*, short, void*);
