#pragma once

#include "Config.h"

#if !defined(_DEBUG) && !defined(NDEBUG)
#define NDEBUG
#endif

#ifdef NDEBUG
#undef MEM_DEBUG
#endif

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
#define __perror(s) fprintf(stderr, "%s: Win32 Error %d(0x%08x)\n", s, GetLastError(), GetLastError())
#define sleep(t) Sleep((t) * 1000)
#define usleep(t) Sleep((t) / 1000)
#ifdef MEM_DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif
#endif

#ifdef __linux
#include <unistd.h>
#define __perror(s) perror(s)
#endif

#if defined(__APPLE__) && defined(__MACH__)
#include <unistd.h>
#define __perror(s) perror(s)
#endif

#ifdef _DEBUG
#define _perror(s) do {__perror(s); /*abort();*/} while (false)
#else
#define _perror(s) __perror(s)
#endif

#if (!defined(ENABLE_IPV4) && !defined(ENABLE_IPV6))
#error Cannot disable ipv4 and ipv6 at the same time.
#endif

#define _log(type, format, ...) do {FILE *__fd = logFile; printTime(__fd); fprintf(__fd, "[%s] ", type); fprintf(__fd, format, ##__VA_ARGS__); fprintf(__fd, "\n");} while (false)
#define log_normal(format, ...) _log("normal", format, ##__VA_ARGS__)
#define log_debug(format, ...) _log("debug", format, ##__VA_ARGS__)
#define log_warn(format, ...) _log("WARN", format, ##__VA_ARGS__)
#define log_error(format, ...) do {_log("ERROR", format, ##__VA_ARGS__); abort();} while (false)
#define log_fatal(format, ...) do {_log("FATAL", format, ##__VA_ARGS__); abort();} while (false)

#define DISALLOW_COPY_AND_ASSIGN(T) \
	T(const T&) = delete;    \
	T& operator=(const T&) = delete;

#ifdef NDEBUG
#undef log_debug
#define log_debug(format, ...) (int)0
#endif

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <event2/event.h>
#include <event2/thread.h>
#include <event2/bufferevent.h>

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <thread>
#include <sstream>
#include <mutex>

using namespace std;
using rapidjson::Document;
using rapidjson::Value;
using rapidjson::Writer;
using rapidjson::StringBuffer;

extern FILE *logFile; // defined in MagicCubeServer.cpp

#include "../CubeCommon/CubeCommon.h"

#include "utilities.h"
#include "MagicCubeServer.h"
