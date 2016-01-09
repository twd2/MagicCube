// MagicCubeServer.cpp : ��������̨Ӧ�ó��������ڵ㡣
//

#include "stdafx.h"
#include "MagicCubeServer.h"

int main(int argc, char *argv[])
{
#ifdef WIN32
	WSADATA data;
	int err = WSAStartup(0, &data);
	err = WSAStartup(data.wVersion, &data);
#endif
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
	sin.sin_addr.s_addr = 0;
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

	event_base *base = event_base_new();
	if (!base)
	{
		perror("base");
		return 1;
	}
	event *listen_event;
	listen_event = event_new(base, listener, EV_READ | EV_PERSIST, accept_cb, (void*)base);
	event_add(listen_event, NULL);
	
	thread th(eventEntry, base);
	th.join();

	printf("The End.");
	event_free(listen_event);
	event_base_free(base);
	return 0;
}

void eventEntry(event_base *base)
{
	event_base_dispatch(base);
}

map<int, bool> sent;

void accept_cb(evutil_socket_t listener, short event, void *arg)
{
	event_base *base = (event_base *)arg;
	evutil_socket_t fd;
	sockaddr_in sin;
	socklen_t slen = (socklen_t)sizeof(sin);
	fd = accept(listener, (sockaddr *)&sin, &slen);
	if (fd < 0) {
		perror("accept");
		return;
	}
	sent[fd] = false;

	char str[INET6_ADDRSTRLEN] = { 0 };
	evutil_inet_ntop(sin.sin_family, &(sin.sin_addr), str, INET6_ADDRSTRLEN);
	printf("ACCEPT: fd = %u, addr = %s\n", fd, str);

	bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, read_cb, write_cb, error_cb, arg);
	bufferevent_enable(bev, EV_READ | EV_WRITE | EV_PERSIST);
}

void read_cb(bufferevent *bev, void *arg)
{
#define MAX_LINE    255
	char line[MAX_LINE + 1];
	size_t n = 0;
	evutil_socket_t fd = bufferevent_getfd(bev);

	while ((n = bufferevent_read(bev, line, MAX_LINE)) > 0)
	{
		line[n] = '\0';
		printf("fd = %u, read line: %s\n", fd, line);
	}

	if (!sent[fd])
	{
		string content = string("<h1>It works!</h1>\n<p>") + to_string(rand()) + "</p>";
		string head = string("HTTP/1.0 200 OK\r\nServer: Wandai\r\nContent-Type: text/html\r\nTransfer-Encoding: identity\r\nContent-Length: ") + to_string(content.length()) + "\r\nConnection: close\r\n\r\n";

		string data = head + content;

		bufferevent_write(bev, data.c_str(), data.length());
		sent[fd] = true;
		bufferevent_flush(bev, EV_WRITE, BEV_NORMAL);
		//shutdown(fd, 0);
	}
}

void write_cb(bufferevent *bev, void *arg) 
{

}

void error_cb(bufferevent *bev, short event, void *arg)
{
	evutil_socket_t fd = bufferevent_getfd(bev);
	printf("fd = %u, ", fd);
	if (event & BEV_EVENT_TIMEOUT)
	{
		printf("timed out\n"); //if bufferevent_set_timeouts() called
	}
	else if (event & BEV_EVENT_EOF)
	{
		printf("connection closed\n");
	}
	else if (event & BEV_EVENT_ERROR)
	{
		printf("some other error\n");
	}
	bufferevent_free(bev);
}

