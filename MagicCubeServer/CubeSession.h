#pragma once

#include "Session.h"

class CubeSession :
	public Session
{
public:

#ifdef ENABLE_IPV4
	CubeSession(TcpServer&, sockaddr_in, evutil_socket_t);
#endif
#ifdef ENABLE_IPV6
	CubeSession(TcpServer&, sockaddr_in6, evutil_socket_t);
#endif

	~CubeSession();


	void OnPackage(Package*&);

	void SendError(SessionErrorType);
};

