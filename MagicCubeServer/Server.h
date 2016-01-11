#pragma once

#include "Session.h"

class Server
{
public:
	event_base *Base;

	vector<Session*> Sessions;

	Server();

#ifdef ENABLE_IPV4
	bool Listen(string, unsigned short, int);
	void AcceptCallback(short);
	void Stop();
#endif

#ifdef ENABLE_IPV6
	bool Listen6(string, unsigned short, int);
	void Accept6Callback(short);
	void Stop6();
#endif

	void Start();
	void EnableTimer(long);
	void TimerCallback(short);

	~Server();

private:

	Server(Server&);

#ifdef ENABLE_IPV4
	event *listener_event;
	evutil_socket_t listener = NULL;
#endif

#ifdef ENABLE_IPV6
	event *listener6_event;
	evutil_socket_t listener6 = NULL;
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
