#include "stdafx.h"
#include "CubeClient.h"

CubeClient::CubeClient()
	: TcpClient()
{
	initCommandHandlers();
}


CubeClient::~CubeClient()
{
	Close();
}

void CubeClient::Start()
{
	TcpClient::Start();
	threadHeartbeat = new thread([this] {this->heartbeat(); });
}

void CubeClient::Close()
{
	IsAlive = false;

	if (threadHeartbeat)
	{
		if (threadHeartbeat->native_handle())
		{
			threadHeartbeat->join();
		}

		delete threadHeartbeat;
		threadHeartbeat = NULL;
	}

	TcpClient::Close();
}

bool CubeClient::Auth(string key)
{
	StringBuffer sb;

	rapidjson::Writer<StringBuffer> writer(sb);
	writer.StartObject();

	writer.String("command");
	writer.String("auth");

	writer.String("key");
	writer.String(key.c_str());

	writer.EndObject();

	responseEvent.Reset();
	SendPackage(sb.GetString());
	responseEvent.Wait();

	return Succeeded;
}

void CubeClient::OnPackage(Package *&pack)
{
	if (!pack || pack->Header.DataLength == 0) return;
	pack->Data[pack->Header.DataLength - 1] = '\0';
	log_debug("on package (fd = %u): %s", static_cast<unsigned int>(fd), pack->Data);

	Document doc;
	doc.Parse(pack->Data);
	//  {"command": "cmd...", arg1: argv1, arg2: argv2, ...}
	//  {"ack": "cmd...", arg1: argv1, arg2, argv2, ...}
	//  {"error": 123, "message": "..."}
	//  {"success": 0}
	do
	{
		if (!doc.IsObject())
		{
			break;
		}

		if (doc.HasMember("command") || doc.HasMember("ack"))
		{
			handleCommandAndAck(doc);
		}
		else if (doc.HasMember("success"))
		{
			Succeeded = true;
			responseEvent.Set();
		}
		else if (doc.HasMember("error"))
		{
			Succeeded = false;
			LastErrorCode = _SE(doc["error"].GetInt());
			LastErrorMessage = doc["message"].GetString();
			responseEvent.Set();
		}
	} while (false);

	delete pack;
	pack = NULL;
}

void CubeClient::initCommandHandlers()
{
#define HAND(a) commandHandlers[#a] = &CubeClient::a##Handler
	/*HAND(auth_ack);
	HAND(list_rooms_ack);
	HAND(get_room_info_ack);*/
#undef HAND
}

void CubeClient::handleCommandAndAck(Document &doc)
{
	string cmd;

	if (doc.HasMember("command"))
	{
		cmd = doc["command"].GetString();
	}
	else if (doc.HasMember("ack"))
	{
		cmd = doc["command"].GetString() + string("_ack");
	}

	CommandHandlerPtr hand = NULL;
	if ((hand = commandHandlers[cmd]) != NULL)
	{
		(this->*hand)(doc);
	}
	else
	{
		return;
	}
}

#undef reqArg

void CubeClient::heartbeat()
{
	string pack;

	StringBuffer sb;
	rapidjson::Writer<StringBuffer> writer(sb);

	writer.StartObject();

	writer.String("command");
	writer.String("heartbeat");

	writer.EndObject();

	pack = sb.GetString();

	while (IsAlive)
	{
		SendPackage(pack);
		sleep(1);
		// sleep(TIMEOUT_S - 5);
	}
}