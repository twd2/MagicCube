#pragma once

#include <list>
#include <iterator>
#include "Session.h"

class TcpServer
{
public:
	bool IsRunning = false;

	event_base *Base;

	size_t MaxConnections = static_cast<size_t>(-1);
	list<Session*> Sessions;
	
	TcpServer();

#ifdef ENABLE_IPV4
	bool Listen(string, unsigned short, int);
	virtual void AcceptCallback(short);
	virtual void OnNewSession(sockaddr_in, evutil_socket_t);
#endif

#ifdef ENABLE_IPV6
	bool Listen6(string, unsigned short, int);
	virtual void Accept6Callback(short);
	virtual void OnNewSession(sockaddr_in6, evutil_socket_t);
#endif

	// sync, block
	void Start();
	void Stop();

	void SetTimer(event*&, long);
	void ClearTimer(event*&);
	void EnableTimer(long);

	virtual void TimerCallback(short);
	virtual void CleanSession(Session*);
	virtual void CleanSessions();

	~TcpServer();

protected:

#ifdef ENABLE_IPV4
	event *listener_event = NULL;
	evutil_socket_t listener = static_cast<evutil_socket_t>(0);
#endif

#ifdef ENABLE_IPV6
	event *listener6_event = NULL;
	evutil_socket_t listener6 = static_cast<evutil_socket_t>(0);
#endif

	event *defaultTimer = NULL;

private:

	// prevent copy
	DISALLOW_COPY_AND_ASSIGN(TcpServer);
};

#ifdef ENABLE_IPV4
void acceptCallbackDispatcher(evutil_socket_t, short, void*);
#endif

#ifdef ENABLE_IPV6
void accept6CallbackDispatcher(evutil_socket_t, short, void*);
#endif

void timerCallbackDispatcher(evutil_socket_t, short, void*);
