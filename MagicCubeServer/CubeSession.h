#pragma once

#include "Session.h"

class CubeServer;

class CubeSession :
	public Session
{

public:
	
	friend class RoomInfo;

	typedef void (CubeSession::*CommandHandlerPtr)(Value&);

	bool Authed = false;

	Cube cube;

	CubeServer &Server;

#ifdef ENABLE_IPV4
	CubeSession(CubeServer&, sockaddr_in, evutil_socket_t);
#endif
#ifdef ENABLE_IPV6
	CubeSession(CubeServer&, sockaddr_in6, evutil_socket_t);
#endif

	~CubeSession();

	void OnPackage(Package*&);

	void SendSuccess();
	void SendError(SessionErrorType);
	void SendError(SessionErrorType, bool close);

	bool EnterRoom(string);
	bool EnterRoom(size_t);
	void ExitRoom();

private:

	map<string, CommandHandlerPtr> commandHandlers;
	void initCommandHandlers();

	void handleCommand(Document&);

	ptrdiff_t currentRoom = -1;
	list<CubeSession*>::iterator iterInRoom;

	void authHandler(Value&);
	void list_roomsHandler(Value&);
	void get_room_infoHandler(Value&);
	void heartbeatHandler(Value&);
};
