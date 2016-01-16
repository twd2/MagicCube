#pragma once

#include "CubeSession.h"
#include "TcpServer.h"

class CubeServer :
	public TcpServer
{
public:
	CubeServer();
	~CubeServer();

#ifdef ENABLE_IPV4
	void OnNewSession(sockaddr_in, evutil_socket_t);
#endif

#ifdef ENABLE_IPV6
	void OnNewSession(sockaddr_in6, evutil_socket_t);
#endif

};

