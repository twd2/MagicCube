#include "stdafx.h"
#include "utilities.h"

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

void configServer(CubeServer &server, Value &config)
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

	server.NeedAuth = config["Auth"].GetBool();
	server.ServerKey = config["ServerKey"].GetString();
}

vector<RoomInfo> loadRooms(Value &rooms)
{
	log_normal("%s", "Loading rooms...");

	size_t count = rooms.Size();
	vector<RoomInfo> ris;

	for (size_t i = 0; i < count; ++i)
	{
		Value &room = rooms[i];

		RoomInfo ri;
		ri.Name = room["Name"].GetString();
		ri.NeedAuth = room["Auth"].GetBool();
		ri.Key = room["Key"].GetString();
		ri.Capacity = getSizeT(room["Capacity"]);
		ris.push_back(move(ri));
		log_normal("%s...", room["Name"].GetString());
	}

	return ris;
}

void configRooms(CubeServer &server, vector<RoomInfo> &rooms)
{
	server.Rooms = move(rooms);
	server.RoomIds.clear();
	for (size_t i = 0; i < server.Rooms.size(); ++i)
	{
		server.RoomIds[server.Rooms[i].Name] = i;
	}
}

bool checkObj(Value &doc, size_t argc, ...)
{
	va_list ap = NULL;
	va_start(ap, argc);

	bool succeeded = true;

	for (size_t i = 0; i < argc; ++i)
	{
		const char *name = va_arg(ap, const char*);
		const char *type = va_arg(ap, const char*);

		if (!doc.HasMember(name))
		{
			succeeded = false;
			break;
		}

		if (type == string("str"))
		{
			if (!doc[name].IsString())
			{
				succeeded = false;
				break;
			}
		}
		else if (type == string("int"))
		{
			if (!doc[name].IsInt())
			{
				succeeded = false;
				break;
			}
		}
		else if (type == string("uint"))
		{
			if (!doc[name].IsUint())
			{
				succeeded = false;
				break;
			}
		}
		else if (type == string("int64"))
		{
			if (!doc[name].IsInt64())
			{
				succeeded = false;
				break;
			}
		}
		else if (type == string("uint64"))
		{
			if (!doc[name].IsUint64())
			{
				succeeded = false;
				break;
			}
		}
		else if (type == string("float"))
		{
			if (!doc[name].IsDouble())
			{
				succeeded = false;
				break;
			}
		}
		else if (type == string("bool"))
		{
			if (!doc[name].IsBool())
			{
				succeeded = false;
				break;
			}
		}
		else if (type == string("null"))
		{
			if (!doc[name].IsNull())
			{
				succeeded = false;
				break;
			}
		}
		else if (type == string("array"))
		{
			if (!doc[name].IsArray())
			{
				succeeded = false;
				break;
			}
		}
		else if (type == string("obj"))
		{
			if (!doc[name].IsObject())
			{
				succeeded = false;
				break;
			}
		}
	}

	va_end(ap);

	return succeeded;
}