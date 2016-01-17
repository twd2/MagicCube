#include "stdafx.h"
#include "CubeSession.h"

#ifdef ENABLE_IPV4
CubeSession::CubeSession(CubeServer &server, sockaddr_in addr, evutil_socket_t fd)
	: Session(server, addr, fd), server(server)
{

}
#endif

#ifdef ENABLE_IPV6
CubeSession::CubeSession(CubeServer &server, sockaddr_in6 addr, evutil_socket_t fd)
	: Session(server, addr, fd), server(server)
{

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
	if (!doc.IsObject())
	{
		SendError(SESSIONERROR_PROTOCOL_MISMATCH);
		return;
	}

	SendError(_SE(ERROR_AUTH));
	// TODO: package

	delete pack;
	pack = NULL;
}

void CubeSession::SendError(SessionErrorType errorCode)
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

	FlushAndClose();
}

bool CubeSession::EnterRoom(string name)
{
	return EnterRoom(server.RoomIds[name]);
}

bool CubeSession::EnterRoom(size_t i)
{
	/* 1. Put CubeSession into RoomInfo::Sessions
	 * 2. set currentRoom
	 */

	RoomInfo &room = server.Rooms[i];
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
		server.Rooms[currentRoom].Exit(this);
		currentRoom = -1;
	}
}
