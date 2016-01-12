#include "stdafx.h"
#include "Server.h"

Server::Server()
{
	Base = event_base_new();
	if (!Base)
	{
		_perror("event_base_new");
	}
}

#ifdef ENABLE_IPV4
bool Server::Listen(string address, unsigned short port, int backlog)
{
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener <= 0)
	{
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
		_perror("bind");
		return false;
	}

	if (listen(listener, backlog) < 0)
	{
		_perror("listen");
		return false;
	}

	evutil_make_socket_nonblocking(listener);
	return true;
}

void Server::AcceptCallback(short event)
{
	evutil_socket_t fd;
	sockaddr_in sin;
	socklen_t slen = (socklen_t)sizeof(sin);
	fd = accept(listener, (sockaddr *)&sin, &slen);
	if (fd < 0)
	{
		//_perror("accept");
		return;
	}

	Session *sess = new Session(Base, sin, fd);
	normal("accept fd = %u from %s:%d", fd, sess->RemoteAddress.c_str(), sess->RemotePort);
	sess->SetCallbacks();
	Sessions.push_back(sess);
}

void Server::Stop()
{
	shutdown(listener, SHUT_RDWR);
	close(listener);
}
#endif

#ifdef ENABLE_IPV6
bool Server::Listen6(string address, unsigned short port, int backlog)
{
	listener6 = socket(AF_INET6, SOCK_STREAM, 0);
	if (listener6 <= 0)
	{
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
		_perror("bind6");
		return false;
	}

	if (listen(listener6, backlog) < 0)
	{
		_perror("listen6");
		return false;
	}

	return true;
}

void Server::Accept6Callback(short event)
{
	evutil_socket_t fd;
	sockaddr_in6 sin6;
	socklen_t slen = (socklen_t)sizeof(sin6);
	fd = accept(listener, (sockaddr *)&sin6, &slen);
	if (fd < 0)
	{
		//_perror("accept");
		return;
	}

	Session *sess = new Session(Base, sin6, fd);
	normal("accept fd = %u from [%s]:%d", fd, sess->RemoteAddress.c_str(), sess->RemotePort);
	sess->SetCallbacks();
	Sessions.push_back(sess);
}

void Server::Stop6()
{
	shutdown(listener6, SHUT_RDWR);
	close(listener6);
}
#endif

void Server::Start()
{
#ifdef ENABLE_IPV4
	listener_event = event_new(Base, listener, EV_READ | EV_PERSIST, acceptCallbackDispatcher, (void*)this);
	event_add(listener_event, NULL);
#endif

#ifdef ENABLE_IPV6
	listener6_event = event_new(Base, listener6, EV_READ | EV_PERSIST, accept6CallbackDispatcher, (void*)this);
	event_add(listener6_event, NULL);
#endif

	event_base_dispatch(Base);
}

void Server::EnableTimer(long interval)
{
	timerInterval = new timeval;
	evutil_timerclear(timerInterval);
	timerInterval->tv_sec = interval;

	timer = event_new(Base, -1, EV_PERSIST, timerCallbackDispatcher, (void *)this);
	evtimer_add(timer, timerInterval);
	event_add(timer, NULL);
}

void Server::TimerCallback(short event)
{
	normal("%s", "Timer tick.");

	for (auto &sess : Sessions)
	{
		//sess->SendPackage("server_heartbeat");
		//sess->Close();
	}

	for (ptrdiff_t i = Sessions.size() - 1; i >= 0; --i)
	{
		Session *&sess = Sessions[i];
		normal("Checking %p", sess);
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
			Sessions.erase(Sessions.begin() + i);
		}
	}
}

Server::~Server()
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
	((Server*)arg)->AcceptCallback(event);
}
#endif

#ifdef ENABLE_IPV6
void accept6CallbackDispatcher(evutil_socket_t listener, short event, void *arg)
{
	((Server*)arg)->Accept6Callback(event);
}
#endif

void timerCallbackDispatcher(evutil_socket_t listener, short event, void *arg)
{
	((Server*)arg)->TimerCallback(event);
}