#pragma once

#include "Session.h"

#define _SE(x) (static_cast<SessionErrorType>(static_cast<int>(x)))
#define _UE(x) (static_cast<UserFacingError>(static_cast<int>(x)))

enum UserFacingError
{
	ERROR_AUTH_FAILED = SESSIONERROR_UNKNOWN + 1,
	ERROR_AUTH,
	ERROR_KICKED
};
extern map<UserFacingError, string> UserFacingErrorMessage;

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

