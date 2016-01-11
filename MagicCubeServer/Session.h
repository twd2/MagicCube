#pragma once

#include <queue>
#include "ManualEvent.h"

enum ReadStateType
{
	READSTATE_NONE,
	READSTATE_READING_LENGTH,
	READSTATE_READING_DATA,
	READSTATE_ERROR
};

enum WriteStateType
{
	WRITESTATE_NONE,
	WRITESTATE_WRITING,
	WRITESTATE_ERROR
};

#pragma pack(push, 1)
struct Package
{
	package_len_t length;
	char data[0];
};
#pragma pack(pop)

class Session
{
public:
	bool IsAlive = true;
	bool IsIPv6 = false;
	string RemoteAddress;
	unsigned short RemotePort;

#ifdef ENABLE_IPV4
	Session(event_base*, sockaddr_in, int);
#endif
#ifdef ENABLE_IPV6
	Session(event_base*, sockaddr_in6, int);
#endif
	~Session();

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
#ifndef NDEBUG
	// dummy package, http response
	void SendPackage();
#endif

	// sync, block
	void FlushQueue();

	void Close();

	static Package *MakePackage(string&);

private:
	event_base *evbase;
	bufferevent *buffev;
	evutil_socket_t fd;
#ifdef ENABLE_IPV4
	sockaddr_in sAddr;
#endif
#ifdef ENABLE_IPV6
	sockaddr_in6 sAddr6;
#endif

	ReadStateType readState = READSTATE_NONE;
	size_t readLength;
	char lengthBuffer[sizeof(package_len_t)];
	Package *currentPackage = NULL;

	bool fisrtWriteCallback = true;
	WriteStateType writeState = WRITESTATE_NONE;
	size_t writtenLength;
	queue<Package*> pendingPackages;

	ManualEvent writtenEvent;
	mutex readLock, writeLock;
};

package_len_t htonpacklen(package_len_t);
package_len_t ntohpacklen(package_len_t);
void readCallbackDispatcher(bufferevent*, void*);
void writeCallbackDispatcher(bufferevent*, void*);
void errorCallbackDispatcher(bufferevent*, short, void*);
