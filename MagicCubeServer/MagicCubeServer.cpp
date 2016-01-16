#include "stdafx.h"
#include "MagicCubeServer.h"

FILE *logFile = stdout;

void libeventError(int errcode)
{
	log_fatal("libevent fatal error occurred, error code: %d\n", errcode);
	exit(1);
}

void initLibraries()
{
#ifdef _DEBUG
	event_enable_debug_mode(); // may cause memory leak
#endif

	event_set_fatal_callback(libeventError);

#ifdef _WIN32

#ifdef MEM_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	WSADATA data;
	int err = WSAStartup(0, &data);
	err = WSAStartup(data.wVersion, &data);
#endif

#ifdef EVTHREAD_USE_WINDOWS_THREADS_IMPLEMENTED
	evthread_use_windows_threads(); // may cause memory leak
#endif

#ifdef EVTHREAD_USE_PTHREADS_IMPLEMENTED
	evthread_use_pthreads();
#endif
}

int main(int argc, char *argv[])
{
	string configFilename = "Config.json";
	string logFilename = "-";

#ifndef _WIN32
	cmdline::parser arg;
	arg.add<string>("config", 'c', "configuration file", false, "Config.json");
	arg.add<string>("log", 'l', "log file", false, "-");
	arg.parse_check(argc, argv);
	configFilename = arg.get<string>("config");
	logFilename = arg.get<string>("log");
#endif 

	log_normal("Using configuration file: %s", configFilename.c_str());
	log_normal("Log file: %s", logFilename.c_str());

	setLogFile(logFilename);

	Document configDoc = loadConfigObj(configFilename);
	Value &roomsVal = configDoc["Rooms"];
	assert(roomsVal.IsArray());
	vector<RoomInfo> rooms = loadRooms(roomsVal);

	initLibraries();

	CubeServer server;


	server.EnableTimer(CHECK_INTERVAL);

	configServer(server, configDoc["Server"]);

	server.IsRunning = true;
	thread th(eventEntry, &server);

	initHandlers();
	handleCommand(server);

	server.Stop();
	th.join();

	log_normal("%s", "Stopped.");

#ifdef MEM_DEBUG
#ifdef _WIN32
	_CrtDumpMemoryLeaks();
#endif 
#endif 

	return 0;
}

void eventEntry(TcpServer *server)
{
	server->Start();
}
