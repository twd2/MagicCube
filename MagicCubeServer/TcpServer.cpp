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
		listener = (evutil_socket_t)0;
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
		listener = (evutil_socket_t)0;
		_perror("bind");
		return false;
	}

	if (listen(listener, backlog) < 0)
	{
		listener = (evutil_socket_t)0;
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
	normal("accept fd = %u from %s:%d", (unsigned int)fd, sess->RemoteAddress.c_str(), sess->RemotePort);
	sess->SetCallbacks();
	Sessions.push_back(sess);
	sess->Iter = Sessions.end();
	--sess->Iter;
}

void TcpServer::Stop()
{
	shutdown(listener, SHUT_RDWR);
	close(listener);
}
#endif

#ifdef ENABLE_IPV6
bool TcpServer::Listen6(string address, unsigned short port, int backlog)
{
	listener6 = socket(AF_INET6, SOCK_STREAM, 0);
	if (listener6 <= 0)
	{
		listener6 = (evutil_socket_t)0;
		_perror("socket6");
		return false;
	}
	evutil_make_listen_socket_reuseable(listener6);

	sockaddr_in6 sin6;
	sin6.sin6_family = AF_INET6;
	evutil_inet_pton(AF_INET6, address.c_str(), &(sin6.sin6_addr.s6_addr));
	sin6.sin6_port = htons(port);

	if (::bind(listener6, (sockaddr *)&sin6, sizeof(sin6)) < 0)
	{
		listener6 = (evutil_socket_t)0;
		_perror("bind6");
		return false;
	}

	if (listen(listener6, backlog) < 0)
	{
		listener6 = (evutil_socket_t)0;
		_perror("listen6");
		return false;
	}

	return true;
}

void TcpServer::Accept6Callback(short event)
{
	evutil_socket_t fd;
	sockaddr_in6 sin6;
	socklen_t slen = (socklen_t)sizeof(sin6);
	fd = accept(listener, (sockaddr *)&sin6, &slen);
	if (fd < 0)
	{
		_perror("accept");
		return;
	}

	Session *sess = new Session(*this, sin6, fd);
	normal("accept fd = %u from [%s]:%d", (unsigned int)fd, sess->RemoteAddress.c_str(), sess->RemotePort);
	sess->SetCallbacks();
	Sessions.push_back(sess);
	sess->Iter = Sessions.end();
	--sess->Iter;
}

void TcpServer::Stop6()
{
	shutdown(listener6, SHUT_RDWR);
	close(listener6);
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

	event_base_dispatch(Base);
}

void TcpServer::EnableTimer(long interval)
{
	if (timer)
	{
		event_del(timer);
		event_free(timer);
		timer = NULL;
	
		delete timerInterval;
		timerInterval = NULL;
	}

	timerInterval = new timeval;
	evutil_timerclear(timerInterval);
	timerInterval->tv_sec = interval;

	timer = event_new(Base, -1, EV_PERSIST, timerCallbackDispatcher, (void *)this);
	evtimer_add(timer, timerInterval);
	event_add(timer, NULL);
}

void TcpServer::TimerCallback(short event)
{
	normal("%s", "Timer tick.");

	time_t now = time(NULL);

	for (auto &sess : Sessions)
	{
		if (difftime(now, sess->LastAlive) > TIMEOUT_S)
		{
			sess->SendPackage("Timed out, good bye~");
			sess->FlushAndClose();
			normal("Cleaning %p", sess);
		}
	}

	CleanSessions();
}

void TcpServer::CleanSession(Session *sess)
{
	if (!sess) return;

	//normal("Checking %p", sess);

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
		//normal("Checking %p", sess);
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
	if (timer)
	{
		event_free(timer);
		timer = NULL;
	}
	if (timerInterval)
	{
		delete timerInterval;
		timerInterval = NULL;
	}

	for (auto &sess : Sessions)
	{
		delete sess;
		sess = NULL;
	}
	Sessions.clear();

#ifdef ENABLE_IPV4
	if (listener_event)
	{
		event_free(listener_event);
		listener_event = NULL;
	}
#endif

#ifdef ENABLE_IPV6
	if (listener6_event)
	{
		event_free(listener6_event);
		listener6_event = NULL;
	}
#endif

	if (Base)
	{
		event_base_free(Base);
		Base = NULL;
	}
}

#ifdef ENABLE_IPV4
void acceptCallbackDispatcher(evutil_socket_t listener, short event, void *arg)
{
	((TcpServer*)arg)->AcceptCallback(event);
}
#endif

#ifdef ENABLE_IPV6
void accept6CallbackDispatcher(evutil_socket_t listener, short event, void *arg)
{
	((TcpServer*)arg)->Accept6Callback(event);
}
#endif

void timerCallbackDispatcher(evutil_socket_t listener, short event, void *arg)
{
	((TcpServer*)arg)->TimerCallback(event);
}
