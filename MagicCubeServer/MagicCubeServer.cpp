#include "stdafx.h"
#include "MagicCubeServer.h"

void libeventError(int errcode)
{
	fatal("libevent fatal error occurred, error code: %d\n", errcode);
	exit(1);
}

void init()
{
#ifndef NDEBUG
	//event_enable_debug_mode(); // may cause memory leak
#endif
	event_set_fatal_callback(libeventError);

#ifdef _WIN32
	WSADATA data;
	int err = WSAStartup(0, &data);
	err = WSAStartup(data.wVersion, &data);
	evthread_use_windows_threads(); // may cause memory leak
#ifdef MEM_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#else
	evthread_use_pthreads();
#endif
}

void printTime(FILE *fd)
{
	time_t now = time(NULL);
	char tmpBuf[256];
	tm t;
#ifdef _WIN32
	localtime_s(&t, &now);
#else
	t = *localtime(&now);
#endif
	strftime(tmpBuf, sizeof(tmpBuf), "%Y-%m-%d %H:%M:%S", &t);
	fprintf(fd, "[%s]", tmpBuf);
}

int main(int argc, char *argv[])
{
	printTime(stderr);
	init();

	// while (true)
	{
		Server server;

		server.EnableTimer(CHECK_INTERVAL);

#ifdef ENABLE_IPV4
		server.Listen(LISTEN_ADDR, LISTEN_PORT, LISTEN_BACKLOG);
		normal("Listening %s:%d...", LISTEN_ADDR, LISTEN_PORT);
#endif

#ifdef ENABLE_IPV6
		server.Listen6(LISTEN_ADDR6, LISTEN_PORT6, LISTEN_BACKLOG6);
		normal("Listening [%s]:%d...", LISTEN_ADDR6, LISTEN_PORT6);
#endif
		server.Start();
		thread th(eventEntry, &server);
		getchar();

#ifdef ENABLE_IPV4
		server.Stop();
#endif
#ifdef ENABLE_IPV6
		server.Stop6();
#endif

		event_base_loopbreak(server.Base);

		th.join();

		normal("%s", "Stopped.");

		//server.~Server();
	}

#ifdef MEM_DEBUG
#ifdef _WIN32
	_CrtDumpMemoryLeaks();
#endif 
#endif 

	return 0;
}

void eventEntry(Server *server)
{
	server->Start();
}
