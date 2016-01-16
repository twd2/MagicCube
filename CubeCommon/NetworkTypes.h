#pragma once

#include "stdafx.h"

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
