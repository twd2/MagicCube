#include "stdafx.h"
#include "NetworkTypes.h"

map<SessionErrorType, string> SessionErrorMessage =
{
	{ SESSIONERROR_PROTOCOL_MISMATCH,  "protocol mismatch"   },
	{ SESSIONERROR_PACKAGE_EMPTY,      "package is empty"    },
	{ SESSIONERROR_PACKAGE_TOO_LONG,   "package is too long" },
	{ SESSIONERROR_SERVER_CLOSE,       "server is closing"   },
	{ SESSIONERROR_SERVER_TIMEOUT,     "timed out"           },
	{ SESSIONERROR_UNKNOWN,            "unknown"             }
};

map<UserFacingError, string> UserFacingErrorMessage =
{
	{ ERROR_AUTH_FAILED,  "authorization failed" },
	{ ERROR_AUTH,         "authorization needed" },
	{ ERROR_KICKED,       "kicked by server"     }
};