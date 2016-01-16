#include "stdafx.h"
#include "TcpServer.h"

TcpServer::TcpServer()
{
	Base = event_base_new();
	if (!Base)
	{
		_perror("event_base_new");
	}
}

#ifdef ENABLE_IPV4
bool TcpServer::Listen(string address, unsigned short port, int backlog)
{
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener <= 0)
	{
		listener = static_cast<evutil_socket_t>(0);
		_perror("socket");
		return false;
	}
	evutil_make_listen_socket_reuseable(listener);

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	evutil_inet_pton(AF_INET, address.c_str(), &(sin.sin_addr.s_addr));
	sin.sin_port = htons(port);

	if (::bind(listener, (sockaddr *)&sin, sizeof(sin)) < 0)
	{
		listener = static_cast<evutil_socket_t>(0);
		_perror("bind");
		return false;
	}

	if (listen(listener, backlog) < 0)
	{
		listener = static_cast<evutil_socket_t>(0);
		_perror("listen");
		return false;
	}

	evutil_make_socket_nonblocking(listener);
	return true;
}

void TcpServer::AcceptCallback(short event)
{
	evutil_socket_t fd;
	sockaddr_in sin;
	socklen_t slen = (socklen_t)sizeof(sin);
	fd = accept(listener, (sockaddr *)&sin, &slen);
	if (fd < 0)
	{
		_perror("accept");
		return;
	}

	Session *sess = new Session(*this, sin, fd);
	log_normal("accept fd = %u from %s:%d", static_cast<unsigned int>(fd), sess->RemoteAddress.c_str(), sess->RemotePort);
	sess->SetCallbacks();
	Sessions.push_back(sess);
	sess->Iter = Sessions.end();
	--sess->Iter;
}
#endif

#ifdef ENABLE_IPV6
bool TcpServer::Listen6(string address, unsigned short port, int backlog)
{
	listener6 = socket(AF_INET6, SOCK_STREAM, 0);
	if (listener6 <= 0)
	{
		listener6 = static_cast<evutil_socket_t>(0);
		_perror("socket6");
		return false;
	}
	evutil_make_listen_socket_reuseable(listener6);

#ifndef _WIN32
	int yes = 1;     
	setsockopt(listener6, IPPROTO_IPV6, IPV6_V6ONLY, (void *)&yes, sizeof(yes));
#endif

	sockaddr_in6 sin6;
	sin6.sin6_family = AF_INET6;
	evutil_inet_pton(AF_INET6, address.c_str(), sin6.sin6_addr.s6_addr);
	sin6.sin6_port = htons(port);

	if (::bind(listener6, (sockaddr *)&sin6, sizeof(sin6)) < 0)
	{
		listener6 = static_cast<evutil_socket_t>(0);
		_perror("bind6");
		return false;
	}

	if (listen(listener6, backlog) < 0)
	{
		listener6 = static_cast<evutil_socket_t>(0);
		_perror("listen6");
		return false;
	}

	evutil_make_socket_nonblocking(listener6);
	return true;
}

void TcpServer::Accept6Callback(short event)
{
	evutil_socket_t fd;
	sockaddr_in6 sin6;
	socklen_t slen = (socklen_t)sizeof(sin6);
	fd = accept(listener6, (sockaddr *)&sin6, &slen);
	if (fd < 0)
	{
		_perror("accept6");
		return;
	}

	Session *sess = new Session(*this, sin6, fd);
	log_normal("accept6 fd = %u from [%s]:%d", static_cast<unsigned int>(fd), sess->RemoteAddress.c_str(), sess->RemotePort);
	sess->SetCallbacks();
	Sessions.push_back(sess);
	sess->Iter = Sessions.end();
	--sess->Iter;
}
#endif

void TcpServer::Start()
{
#ifdef ENABLE_IPV4
	if (listener)
	{
		listener_event = event_new(Base, listener, EV_READ | EV_PERSIST, acceptCallbackDispatcher, (void*)this);
		event_add(listener_event, NULL);
	}
#endif

#ifdef ENABLE_IPV6
	if (listener6)
	{
		listener6_event = event_new(Base, listener6, EV_READ | EV_PERSIST, accept6CallbackDispatcher, (void*)this);
		event_add(listener6_event, NULL);
	}
#endif

	IsRunning = true;
	event_base_dispatch(Base);
}

void TcpServer::Stop()
{
#ifdef ENABLE_IPV4
	if (listener)
	{
		shutdown(listener, SHUT_RDWR);
#ifdef _WIN32
		closesocket(listener);
#else
		close(listener);
#endif
		listener = static_cast<evutil_socket_t>(0);
	}

	if (listener_event)
	{
		event_del(listener_event);
		event_free(listener_event);
		listener_event = NULL;
	}
#endif

#ifdef ENABLE_IPV6
	if (listener6)
	{
		shutdown(listener6, SHUT_RDWR);
#ifdef _WIN32
		closesocket(listener6);
#else
		close(listener6);
#endif
		listener6 = static_cast<evutil_socket_t>(0);
	}

	if (listener6_event)
	{
		event_del(listener6_event);
		event_free(listener6_event);
		listener6_event = NULL;
	}
#endif

	event_base_loopbreak(Base);
	IsRunning = false;
}

void TcpServer::SetTimer(event *&timer, long interval)
{
	ClearTimer(timer);

	timeval *timerInterval = new timeval;
	evutil_timerclear(timerInterval);
	timerInterval->tv_sec = interval;

	timer = event_new(Base, -1, EV_PERSIST, timerCallbackDispatcher, (void *)this);
	evtimer_add(timer, timerInterval);

	delete timerInterval;
	timerInterval = NULL;
}

void TcpServer::ClearTimer(event *&timer)
{
	if (timer)
	{
		event_del(timer);
		event_free(timer);
		timer = NULL;
	}
}

void TcpServer::EnableTimer(long interval)
{
	SetTimer(defaultTimer, interval);
}

void TcpServer::TimerCallback(short event)
{
	log_debug("%s", "Timer tick.");

	time_t now = time(NULL);

	for (auto &sess : Sessions)
	{
		if (difftime(now, sess->LastAlive) > TIMEOUT_S)
		{
			sess->SendPackage("Timed out, good bye~"); // TODO: change message
			sess->FlushAndClose();
			log_debug("Cleaning %p", sess);
		}
	}

	CleanSessions();
}

void TcpServer::CleanSession(Session *sess)
{
	if (!sess) return;

	log_debug("Checking %p", sess);

	if (!sess->IsAlive)
	{
		list<Session*>::iterator iter = sess->Iter;
		delete sess;

		Sessions.erase(iter);
	}
}

void TcpServer::CleanSessions()
{
	for (list<Session*>::iterator iter = Sessions.begin(); iter != Sessions.end(); ++iter)
	{
		Session *&sess = *iter;
		log_debug("Checking %p", sess);
		if (sess)
		{
			if (!sess->IsAlive)
			{
				delete sess;
				sess = NULL;
			}
		}
		if (!sess)
		{
			Sessions.erase(iter);
		}
	}
}

TcpServer::~TcpServer()
{
	Stop();

	ClearTimer(defaultTimer);

	for (auto &sess : Sessions)
	{
		delete sess;
		sess = NULL;
	}
	Sessions.clear();

	if (Base)
	{
		event_base_free(Base);
		Base = NULL;
	}
}

#ifdef ENABLE_IPV4
void acceptCallbackDispatcher(evutil_socket_t listener, short event, void *arg)
{
	(reinterpret_cast<TcpServer*>(arg))->AcceptCallback(event);
}
#endif

#ifdef ENABLE_IPV6
void accept6CallbackDispatcher(evutil_socket_t listener, short event, void *arg)
{
	(reinterpret_cast<TcpServer*>(arg))->Accept6Callback(event);
}
#endif

void timerCallbackDispatcher(evutil_socket_t listener, short event, void *arg)
{
	(reinterpret_cast<TcpServer*>(arg))->TimerCallback(event);
}
