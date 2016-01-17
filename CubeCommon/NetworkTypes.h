#pragma once

#include "NetworkConfig.h"
#include "stdafx.h"

#ifdef _WIN32
#ifndef WIN32
#define WIN32 // for libevent
#endif
typedef int socklen_t;
#define INET6_ADDRSTRLEN 46
#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#define SHUT_RDWR SD_BOTH
#ifdef ENABLE_IPV6
// undefining ENABLE_IPV6
#undef ENABLE_IPV6
#endif
#define sleep(t) Sleep((t) * 1000)
#define usleep(t) Sleep((t) / 1000)
#endif

#ifdef __linux
#include <unistd.h>
#endif

#if defined(__APPLE__) && defined(__MACH__)
#include <unistd.h>
#endif

#if (!defined(ENABLE_IPV4) && !defined(ENABLE_IPV6))
#error Cannot disable ipv4 and ipv6 at the same time.
#endif

#define _SE(x) (static_cast<SessionErrorType>(static_cast<int>(x)))
#define _UE(x) (static_cast<UserFacingError>(static_cast<int>(x)))

enum SessionErrorType
{
	SESSIONERROR_PROTOCOL_MISMATCH,
	SESSIONERROR_PACKAGE_EMPTY,
	SESSIONERROR_PACKAGE_TOO_LONG,
	SESSIONERROR_SERVER_CLOSE,
	SESSIONERROR_SERVER_TIMEOUT,
	SESSIONERROR_UNKNOWN
};
extern map<SessionErrorType, string> SessionErrorMessage;

enum UserFacingError
{
	ERROR_AUTH_FAILED = SESSIONERROR_UNKNOWN + 1,
	ERROR_AUTH,
	ERROR_KICKED
};
extern map<UserFacingError, string> UserFacingErrorMessage;
