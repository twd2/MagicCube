#include "stdafx.h"
#include "MagicCubeServer.h"

int main(int argc, char *argv[])
{
#ifdef WIN32
	WSADATA data;
	int err = WSAStartup(0, &data);
	err = WSAStartup(data.wVersion, &data);
#endif

#ifdef ENABLE_IPV4
	evutil_socket_t listener;
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener <= 0)
	{
		perror("socket");
		return 1;
	}
	evutil_make_listen_socket_reuseable(listener);

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	evutil_inet_pton(AF_INET, LISTEN_ADDR, &(sin.sin_addr.s_addr));
	sin.sin_port = htons(LISTEN_PORT);

	if (::bind(listener, (sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("bind");
#ifdef WIN32
		printf("%d\n", GetLastError());
#endif
		return 1;
	}

	if (listen(listener, LISTEN_BACKLOG) < 0)
	{
		perror("listen");
		return 1;
	}

	printf("Listening...\n");

	evutil_make_socket_nonblocking(listener);
#endif

#ifdef ENABLE_IPV6
	evutil_socket_t listener6;
	listener6 = socket(AF_INET6, SOCK_STREAM, 0);
	if (listener6 <= 0)
	{
		perror("socket6");
		return 1;
	}
	evutil_make_listen_socket_reuseable(listener6);

	sockaddr_in6 sin6;
	sin6.sin6_family = AF_INET6;
	evutil_inet_pton(AF_INET6, LISTEN_ADDR6, &(sin6.sin6_addr.s6_addr));
	sin6.sin6_port = htons(LISTEN_PORT6);

	if (::bind(listener6, (sockaddr *)&sin6, sizeof(sin6)) < 0)
	{
		perror("bind6");
#ifdef WIN32
		printf("%d\n", GetLastError());
#endif
		return 1;
	}

	if (listen(listener6, LISTEN_BACKLOG6) < 0)
	{
		perror("listen6");
		return 1;
	}

	printf("Listening6...\n");

	evutil_make_socket_nonblocking(listener6);
#endif

	event_base *base = event_base_new();
	if (!base)
	{
		perror("base");
		return 1;
	}

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

	printf("Press Enter to stop.\n");

	string s;
	getline(cin, s);
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

	printf("bye\n");
#ifdef ENABLE_IPV4
	event_free(listen_event);
#endif

#ifdef ENABLE_IPV6
	event_free(listen6_event);
#endif
	event_base_free(base);
	return 0;
}

void eventEntry(event_base *base)
{
	event_base_dispatch(base);
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
		perror("accept");
		return;
	}

	Session *sess = new Session(base, sin, fd);
	printf("accept fd = %u, addr = %s\n", fd, sess->RemoteAddress.c_str());
	sess->SetCallbacks();
	// TODO: maintain sessions.
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
		perror("accept");
		return;
	}

	Session *sess = new Session(base, sin6, fd);
	printf("accept6 fd = %u, addr = %s\n", fd, sess->RemoteAddress.c_str());
	sess->SetCallbacks();
	// TODO: maintain sessions.
}
#endif