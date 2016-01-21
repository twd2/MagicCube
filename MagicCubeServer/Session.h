#pragma once

#include <list>
#include <iterator>
#include <queue>

class TcpServer;

enum ReadStateType
{
	READSTATE_NONE,
	READSTATE_READING_HEADER,
	READSTATE_READING_DATA,
	READSTATE_READING_LINE,
	READSTATE_ERROR
};

class Session
{
public:
	list<Session*>::iterator Iter;

	bool IsAlive = true;
	bool IsIPv6 = false;
	time_t LastAlive = time(NULL);
	
	string RemoteAddress;
	unsigned short RemotePort;

	TcpServer &Server;

#ifdef ENABLE_IPV4
	Session(TcpServer&, sockaddr_in, evutil_socket_t);
#endif
#ifdef ENABLE_IPV6
	Session(TcpServer&, sockaddr_in6, evutil_socket_t);
#endif
	virtual ~Session();

	// invoked again and again when doing complex calculation to avoid being cleaned
	void KeepAlive();
	
	void SetCallbacks();
	void SetCallbacks(bool, bool, bool);
	void ClearCallbacks();
	
	virtual void ReadCallback();
	virtual void WriteCallback();
	virtual void ErrorCallback(short);
	
	void DoQueue();
	
	virtual void OnLine(const string&);
	virtual void OnHTTPRequest(const string&);
	virtual void OnPackage(Package*&);

	void SendPackage(Package*&);
	void SendPackage(string);
	virtual void SendError(SessionErrorType);

	void FlushAndClose();
	void Close();

	static Package *MakePackage(string&);

protected:

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
	SessionErrorType readErrorCode = SESSIONERROR_UNKNOWN;
	size_t readLength;
	char headerBuffer[sizeof(PackageHeader)];
	string lineBuffer;
	Package *currentPackage = NULL;

	bool isFirstCall = true;
	queue<Package*> pendingPackages;
	mutex writeLock, queueLock;
	bool closeAfterWritten = false;
	
private:

	// prevent copying
	DISALLOW_COPY_AND_ASSIGN(Session);
};

void readCallbackDispatcher(bufferevent*, void*);
void writeCallbackDispatcher(bufferevent*, void*);
void errorCallbackDispatcher(bufferevent*, short, void*);
