#pragma once

#include "Config.h"

#ifdef _WIN32
#include "targetver.h"
typedef int socklen_t;
#define INET6_ADDRSTRLEN 46
#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#define SHUT_RDWR SD_BOTH
#define close closesocket
	#ifdef ENABLE_IPV6
		// undefining ENABLE_IPV6
		#undef ENABLE_IPV6
	#endif
#endif

#ifdef linux
#endif

#ifdef _UNIX
#endif

#if (!defined(ENABLE_IPV4) && !defined(ENABLE_IPV6))
#error Cannot disable ipv4 and ipv6 at the same time.
#endif

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <event2/event.h>
#include <event2/bufferevent.h>

#include <iostream>
#include <map>
#include <thread>
#include <sstream>
#include <mutex>

using namespace std;
using rapidjson::Document;
using rapidjson::Writer;
using rapidjson::StringBuffer;

