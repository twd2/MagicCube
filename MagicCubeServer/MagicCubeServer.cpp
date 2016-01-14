#include "stdafx.h"
#include "MagicCubeServer.h"

FILE *logFile = stdout;

void libeventError(int errcode)
{
	fatal("libevent fatal error occurred, error code: %d\n", errcode);
	exit(1);
}

void initLib()
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

void setLogFile(string filename)
{
	if (filename != "-")
	{
#ifdef _WIN32
		logFile = NULL;
		fopen_s(&logFile, filename.c_str(), "w+");
		if (!logFile) logFile = stdout;
#else
		logFile = NULL;
		logFile = fopen(filename.c_str(), "w+");
		if (!logFile) logFile = stdout;
#endif 
	}
}

Document loadConfigObj(string filename)
{
	string configJson = "", line;
	ifstream configFile(filename);
	while (getline(configFile, line))
	{
		configJson += line + "\n";
	}
	configFile.close();

	Document configDoc;
	configDoc.Parse(configJson.c_str());
	assert(configDoc.IsObject());

	return move(configDoc);
}

void configServer(TcpServer &server, Value &config)
{
	if (sizeof(size_t) == sizeof(unsigned int))
	{
		server.MaxConnections =
			static_cast<size_t>(config["MaxConnections"].GetInt());
	}
	else if (sizeof(size_t) == sizeof(unsigned long long))
	{
		server.MaxConnections =
			static_cast<size_t>(config["MaxConnections"].GetInt64());
	}

#ifdef ENABLE_IPV4
	{
		Value &IPVal = config["IPv4"];
		if (IPVal["Enable"].GetBool())
		{
			normal("Listening %s:%d...", IPVal["Address"].GetString(), IPVal["Port"].GetUint());
			server.Listen(IPVal["Address"].GetString(), IPVal["Port"].GetUint(), IPVal["Backlog"].GetInt());
		}
	}
#endif

#ifdef ENABLE_IPV6
	{
		Value &IPVal = config["IPv6"];
		if (IPVal["Enable"].GetBool())
		{
			normal("Listening [%s]:%d...", IPVal["Address"].GetString(), IPVal["Port"].GetUint());
			server.Listen6(IPVal["Address"].GetString(), IPVal["Port"].GetUint(), IPVal["Backlog"].GetInt());
		}
	}
#endif
}

void loadRooms(Value &rooms)
{
	// TODO: loadRooms
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
	logFilename = arg.get<string>("log")
#endif 

	normal("Using configuration file: %s", configFilename.c_str());
	normal("Log file: %s", logFilename.c_str());

	setLogFile(logFilename);

	Document configDoc = loadConfigObj(configFilename);
	Value &roomsVal = configDoc["Rooms"];
	assert(roomsVal.IsArray());
	loadRooms(roomsVal);
	
	initLib();

	// while (true)
	{
		TcpServer server;

		server.EnableTimer(CHECK_INTERVAL);

		configServer(server, configDoc["Server"]);

		thread th(eventEntry, &server);
		getchar();

		server.Stop();
		th.join();

		normal("%s", "Stopped.");
	}

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
