#pragma once

#include "Session.h"

class CubeServer;

class CubeSession :
	public Session
{

public:

	friend class RoomInfo;

	CubeServer &server;

#ifdef ENABLE_IPV4
	CubeSession(CubeServer&, sockaddr_in, evutil_socket_t);
#endif
#ifdef ENABLE_IPV6
	CubeSession(CubeServer&, sockaddr_in6, evutil_socket_t);
#endif

	~CubeSession();

	void OnPackage(Package*&);

	void SendError(SessionErrorType);

	bool EnterRoom(string);
	bool EnterRoom(size_t);
	void ExitRoom();

private:

	ptrdiff_t currentRoom = -1;
	list<CubeSession*>::iterator iterInRoom;
};
