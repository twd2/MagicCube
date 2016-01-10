#include "stdafx.h"
#include "MagicCubeServer.h"

void libeventError(int errcode)
{
	fatal("libevent fatal error occurred, error code: %d\n", errcode);
	exit(1);
}

vector<Session*> Sessions;

int main(int argc, char *argv[])
{
#ifndef NDEBUG
	event_enable_debug_mode();
#endif

	event_set_fatal_callback(libeventError);

#ifdef _WIN32
	WSADATA data;
	int err = WSAStartup(0, &data);
	err = WSAStartup(data.wVersion, &data);
	evthread_use_windows_threads();
#else
	evthread_use_pthreads();
#endif

#ifdef ENABLE_IPV4
	evutil_socket_t listener;
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener <= 0)
	{
		_perror("socket");
		return 1;
	}
	evutil_make_listen_socket_reuseable(listener);

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	evutil_inet_pton(AF_INET, LISTEN_ADDR, &(sin.sin_addr.s_addr));
	sin.sin_port = htons(LISTEN_PORT);

	if (::bind(listener, (sockaddr *)&sin, sizeof(sin)) < 0)
	{
		_perror("bind");
		return 1;
	}

	if (listen(listener, LISTEN_BACKLOG) < 0)
	{
		_perror("listen");
		return 1;
	}

	normal("Listening %s:%d...\n", LISTEN_ADDR, LISTEN_PORT);

	evutil_make_socket_nonblocking(listener);
#endif

#ifdef ENABLE_IPV6
	evutil_socket_t listener6;
	listener6 = socket(AF_INET6, SOCK_STREAM, 0);
	if (listener6 <= 0)
	{
		_perror("socket6");
		return 1;
	}
	evutil_make_listen_socket_reuseable(listener6);

	sockaddr_in6 sin6;
	sin6.sin6_family = AF_INET6;
	evutil_inet_pton(AF_INET6, LISTEN_ADDR6, &(sin6.sin6_addr.s6_addr));
	sin6.sin6_port = htons(LISTEN_PORT6);

	if (::bind(listener6, (sockaddr *)&sin6, sizeof(sin6)) < 0)
	{
		_perror("bind6");
		return 1;
	}

	if (listen(listener6, LISTEN_BACKLOG6) < 0)
	{
		_perror("listen6");
		return 1;
	}

	normal("Listening [%s]:%d...\n", LISTEN_ADDR6, LISTEN_PORT6);

	evutil_make_socket_nonblocking(listener6);
#endif

	event_base *base = event_base_new();
	if (!base)
	{
		_perror("event_base_new");
		return 1;
	}

	timeval *timerInterval = new timeval;
	evutil_timerclear(timerInterval);
	timerInterval->tv_sec = CHECK_INTERVAL;

	event *timer = event_new(base, -1, EV_PERSIST, timer_cb, (void *)base);
	evtimer_add(timer, timerInterval);
	event_add(timer, NULL);

#ifdef ENABLE_IPV4
	event *listen_event;
	listen_event = event_new(base, listener, EV_READ | EV_PERSIST, accept_cb, (void*)base);
	event_add(listen_event, NULL);
#endif

#ifdef ENABLE_IPV6
	event *listen6_event;
	listen6_event = event_new(base, listener6, EV_READ | EV_PERSIST, accept6_cb, (void*)base);
	event_add(listen6_event, NULL);
#endif
	
	thread th(eventEntry, base);
	getchar();

#ifdef ENABLE_IPV4
	shutdown(listener, SHUT_RDWR);
	close(listener);
#endif
#ifdef ENABLE_IPV6
	shutdown(listener6, SHUT_RDWR);
	close(listener6);
#endif

	event_base_loopbreak(base);

	th.join();

	normal("%s", "Stopped.");
	event_free(timer);
	delete timerInterval;
	timerInterval = NULL;
#ifdef ENABLE_IPV4
	event_free(listen_event);
	listen_event = NULL;
#endif

#ifdef ENABLE_IPV6
	event_free(listen6_event);
	listen6_event = NULL;
#endif
	event_base_free(base);
	base = NULL;
	return 0;
}

void eventEntry(event_base *base)
{
	event_base_dispatch(base);
}

void wtf(Session *sess)
{
	sess->SendPackage(string("{\"type\": \"hello\"}"));
	sess->FlushQueue();
	sess->Close();
	delete sess;
	normal("Session at %p has been deleted.", sess);
}

void timer_cb(evutil_socket_t listener, short event, void *arg)
{
	normal("%s", "Timer tick.");

	for (auto &sess : Sessions)
	{
		//sess->SendPackage("server_heartbeat");
		//sess->Close();
	}
	
	if (Sessions.size() > 0)
	{
		for (auto iter = Sessions.end() - 1; iter >= Sessions.begin(); --iter)
		{
			Session *&sess = *iter;
			normal("Checking %p", sess);
			if (sess)
			{
				if (!sess->Alive)
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
}

#ifdef ENABLE_IPV4
void accept_cb(evutil_socket_t listener, short event, void *arg)
{
	event_base *base = (event_base *)arg;
	evutil_socket_t fd;
	sockaddr_in sin;
	socklen_t slen = (socklen_t)sizeof(sin);
	fd = accept(listener, (sockaddr *)&sin, &slen);
	if (fd < 0)
	{
		_perror("accept");
		return;
	}

	/*shutdown(fd, SHUT_RDWR);
	close(fd);
	return;*/
	Session *sess = new Session(base, sin, fd);
	normal("accept fd = %u from %s:%d", fd, sess->RemoteAddress.c_str(), sess->RemotePort);
	sess->SetCallbacks();
	Sessions.push_back(sess);
}
#endif

#ifdef ENABLE_IPV6
void accept6_cb(evutil_socket_t listener, short event, void *arg)
{
	event_base *base = (event_base *)arg;
	evutil_socket_t fd;
	sockaddr_in6 sin6;
	socklen_t slen = (socklen_t)sizeof(sin6);
	fd = accept(listener, (sockaddr *)&sin6, &slen);
	if (fd < 0)
	{
		_perror("accept");
		return;
	}

	Session *sess = new Session(base, sin6, fd);
	normal("accept fd = %u from [%s]:%d", fd, sess->RemoteAddress.c_str(), sess->RemotePort);
	sess->SetCallbacks();
	Sessions.push_back(sess);
}
#endif