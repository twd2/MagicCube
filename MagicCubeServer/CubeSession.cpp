#include "stdafx.h"
#include "CubeSession.h"

#ifdef ENABLE_IPV4
CubeSession::CubeSession(CubeServer &server, sockaddr_in addr, evutil_socket_t fd)
	: Session(server, addr, fd), Server(server)
{
	initCommandHandlers();
}
#endif

#ifdef ENABLE_IPV6
CubeSession::CubeSession(CubeServer &server, sockaddr_in6 addr, evutil_socket_t fd)
	: Session(server, addr, fd), server(server)
{
	initCommandHandlers();
}
#endif

CubeSession::~CubeSession()
{

}

void CubeSession::OnPackage(Package *&pack)
{
	if (!pack || pack->Header.DataLength == 0) return;
	pack->Data[pack->Header.DataLength - 1] = '\0';
	log_debug("on package (fd = %u): %s", static_cast<unsigned int>(fd), pack->Data);

	Document doc;
	doc.Parse(pack->Data);
	//  {"command": "cmd...", arg1: argv1, arg2: argv2, ...}
	do
	{
		if (!doc.IsObject())
		{
			SendError(SESSIONERROR_PROTOCOL_MISMATCH);
			break;
		}

		if (checkObj(doc, 1, "command", "str"))
		{
			handleCommand(doc);
		}
		else
		{
			SendError(SESSIONERROR_PROTOCOL_MISMATCH);
			break;
		}
	}
	while (false);

	delete pack;
	pack = NULL;
}

void CubeSession::SendSuccess()
{
	log_debug("sending success fd = %u", static_cast<unsigned int>(fd));

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);

	writer.StartObject();

	writer.String("success");
	writer.Int(0);

	writer.EndObject();

	SendPackage(buffer.GetString());
}

void CubeSession::SendError(SessionErrorType errorCode)
{
	SendError(errorCode, false);
}

void CubeSession::SendError(SessionErrorType errorCode, bool close)
{
	log_debug("sending error (fd = %u): %d", static_cast<unsigned int>(fd), errorCode);

	int code = static_cast<int>(errorCode);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);

	writer.StartObject();

	writer.String("error");
	writer.Int(errorCode);

	writer.String("message");

	if (code > SESSIONERROR_UNKNOWN) // user facing error
	{
		writer.String(UserFacingErrorMessage[_UE(errorCode)].c_str());
	}
	else
	{
		writer.String(SessionErrorMessage[errorCode].c_str());
	}

	writer.EndObject();

	SendPackage(buffer.GetString());

	//                                 is SessionError
	if (close || static_cast<int>(errorCode) <= static_cast<int>(SESSIONERROR_UNKNOWN))
	{
		FlushAndClose();
	}
}

bool CubeSession::EnterRoom(string name)
{
	return EnterRoom(Server.RoomIds[name]);
}

bool CubeSession::EnterRoom(size_t i)
{
	/* 1. Put CubeSession into RoomInfo::Sessions
	 * 2. set currentRoom
	 */

	RoomInfo &room = Server.Rooms[i];
	bool v = room.Enter(this);

	if (v)
	{
		currentRoom = i;
	}

	return v;
}

void CubeSession::ExitRoom()
{
	/* 1. Remove CubeSession from RoomInfo
	 * 2. Clear currentRoom
	 */

	if (currentRoom >= 0)
	{
		Server.Rooms[currentRoom].Exit(this);
		currentRoom = -1;
	}
}

void CubeSession::initCommandHandlers()
{
#define HAND(a) commandHandlers[#a] = &CubeSession::a##Handler;
	HAND(auth);
	HAND(list_rooms);
	HAND(get_room_info)
#undef HAND
}

void CubeSession::handleCommand(Document &doc)
{
	string cmd = doc["command"].GetString();

	if (cmd != "auth" && !Authed)
	{
		SendError(_SE(ERROR_AUTH));
		return;
	}

	CommandHandlerPtr hand = NULL;
	if ((hand = commandHandlers[cmd]) != NULL)
	{
		(this->*hand)(doc);
	}
	else
	{
		SendError(SESSIONERROR_PROTOCOL_MISMATCH);
		return;
	}
}

// require argument(s)
#define reqArg(v, argc, ...) \
	if (!checkObj((v), (argc), ##__VA_ARGS__)) \
	{ \
		SendError(_SE(ERROR_BAD_ARGUMENT)); \
		return; \
	} \

void CubeSession::authHandler(Value &v)
{
	reqArg(v, 1, "key", "str");

	if (v["key"].GetString() == Server.ServerKey || !Server.NeedAuth)
	{
		Authed = true;
		SendSuccess();
	}
	else
	{
		SendError(_SE(ERROR_AUTH_FAILED));
	}
}

void CubeSession::list_roomsHandler(Value &v)
{
	// TODO
}

void CubeSession::get_room_infoHandler(Value &v)
{
	reqArg(v, 1, "id", "int");
	// TODO
}

#undef reqArg
