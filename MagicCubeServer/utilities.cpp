#include "stdafx.h"
#include "utilities.h"

bool endsWith(const string &fullString, const string &ending)
{
	if (fullString.length() >= ending.length())
	{
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else
	{
		return false;
	}
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

void setLogFile(string filename)
{
	if (filename != "-")
	{
		logFile = NULL;
#ifdef _WIN32
		fopen_s(&logFile, filename.c_str(), "w+");
#else
		logFile = fopen(filename.c_str(), "w+");
#endif 
		if (!logFile)
		{
			logFile = stdout;
			_perror("fopen");
			log_warn("%s", "open log file error");
		}
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

	if (configJson == "")
	{
		log_error("config file is not found or empty");
	}

	Document configDoc;
	configDoc.Parse(configJson.c_str());
	assert(configDoc.IsObject());

	return move(configDoc);
}

size_t getSizeT(Value &v)
{
	if (sizeof(size_t) == sizeof(unsigned int))
	{
		return static_cast<size_t>(v.GetInt());
	}
	else if (sizeof(size_t) == sizeof(unsigned long long))
	{
		return static_cast<size_t>(v.GetInt64());
	}
}

void configServer(TcpServer &server, Value &config)
{
	server.MaxConnections = getSizeT(config["MaxConnections"]);

#ifdef ENABLE_IPV4
	{
		Value &IPVal = config["IPv4"];
		if (IPVal["Enable"].GetBool())
		{
			log_normal("Listening %s:%d...", IPVal["Address"].GetString(), IPVal["Port"].GetUint());
			server.Listen(IPVal["Address"].GetString(), IPVal["Port"].GetUint(), IPVal["Backlog"].GetInt());
		}
	}
#endif

#ifdef ENABLE_IPV6
	{
		Value &IPVal = config["IPv6"];
		if (IPVal["Enable"].GetBool())
		{
			log_normal("Listening [%s]:%d...", IPVal["Address"].GetString(), IPVal["Port"].GetUint());
			server.Listen6(IPVal["Address"].GetString(), IPVal["Port"].GetUint(), IPVal["Backlog"].GetInt());
		}
	}
#endif
}

vector<RoomInfo> loadRooms(Value &rooms)
{
	vector<RoomInfo> ris;
	size_t count = rooms.Size();

	for (int i = 0; i < count; ++i)
	{
		Value &room = rooms[i];

		RoomInfo ri;
		ri.Name = room["Name"].GetString();
		ri.Key = room["Key"].GetString();
		ri.Capacity = getSizeT(room["Capacity"]);
		ris.push_back(move(ri));
	}

	return ris;
}

void configRooms(CubeServer &server, vector<RoomInfo> &rooms)
{
	server.Rooms = rooms;
}