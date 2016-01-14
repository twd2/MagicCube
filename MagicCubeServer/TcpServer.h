#pragma once

#include <list>
#include <iterator>
#include "Session.h"

class TcpServer
{
public:
	event_base *Base;

	size_t MaxConnections = static_cast<size_t>(-1);
	list<Session*> Sessions;
	
	TcpServer();

#ifdef ENABLE_IPV4
	bool Listen(string, unsigned short, int);
	void AcceptCallback(short);
#endif

#ifdef ENABLE_IPV6
	bool Listen6(string, unsigned short, int);
	void Accept6Callback(short);
#endif

	// sync, block
	void Start();
	void Stop();

	void EnableTimer(long);

	void TimerCallback(short);
	void CleanSession(Session*);
	void CleanSessions();

	~TcpServer();

private:

	// prevent copy
	DISALLOW_COPY_AND_ASSIGN(TcpServer);

#ifdef ENABLE_IPV4
	event *listener_event = NULL;
	evutil_socket_t listener = static_cast<evutil_socket_t>(0);
#endif

#ifdef ENABLE_IPV6
	event *listener6_event = NULL;
	evutil_socket_t listener6 = static_cast<evutil_socket_t>(0);
#endif

	timeval *timerInterval = NULL;
	event *timer = NULL;
};

#ifdef ENABLE_IPV4
void acceptCallbackDispatcher(evutil_socket_t, short, void*);
#endif

#ifdef ENABLE_IPV6
void accept6CallbackDispatcher(evutil_socket_t, short, void*);
#endif

void timerCallbackDispatcher(evutil_socket_t, short, void*);
