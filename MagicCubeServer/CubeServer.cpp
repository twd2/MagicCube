#include "stdafx.h"
#include "CubeServer.h"

CubeServer::CubeServer()
	: TcpServer()
{

}


CubeServer::~CubeServer()
{

}

#ifdef ENABLE_IPV4
void CubeServer::OnNewSession(sockaddr_in sin, evutil_socket_t fd)
{
	Session *sess = new CubeSession(*this, sin, fd);
	log_normal("accept fd = %u from %s:%d", static_cast<unsigned int>(fd), sess->RemoteAddress.c_str(), sess->RemotePort);
	sess->SetCallbacks();
	Sessions.push_back(sess);
	sess->Iter = Sessions.end();
	--sess->Iter;
}
#endif

#ifdef ENABLE_IPV6
void CubeServer::OnNewSession(sockaddr_in6 sin6, evutil_socket_t fd)
{
	Session *sess = new CubeSession(*this, sin6, fd);
	log_normal("accept6 fd = %u from [%s]:%d", static_cast<unsigned int>(fd), sess->RemoteAddress.c_str(), sess->RemotePort);
	sess->SetCallbacks();
	Sessions.push_back(sess);
	sess->Iter = Sessions.end();
	--sess->Iter;
}
#endif