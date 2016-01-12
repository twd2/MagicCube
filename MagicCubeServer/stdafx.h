#pragma once

#include "Config.h"

#ifdef NDEBUG
#undef MEM_DEBUG
#endif

#ifdef _WIN32
#ifndef WIN32
#define WIN32
#endif
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
#define __perror(s) fprintf(stderr, "%s: Win32 Error %d(0x%08x)\n", s, GetLastError(), GetLastError())
#ifdef MEM_DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif
#endif

#ifdef linux
#define __perror(s) perror(s)
#endif

#if defined(__APPLE__) && defined(__MACH__)
#define __perror(s) perror(s)
#endif

#ifdef _UNIX
#define __perror(s) perror(s)
#endif

#ifndef NDEBUG
#define _perror(s) do {__perror(s); /*abort();*/} while (false)
#else
#define _perror(s) __perror(s)
#endif

#if (!defined(ENABLE_IPV4) && !defined(ENABLE_IPV6))
#error Cannot disable ipv4 and ipv6 at the same time.
#endif

#define _log(type, format, ...) do {FILE *__fd = logfile; printTime(__fd); fprintf(__fd, "[%s] ", type); fprintf(__fd, format, ##__VA_ARGS__); fprintf(__fd, "\n");} while (false)
#define normal(format, ...) _log("normal", format, ##__VA_ARGS__)
#define debug(format, ...) _log("debug", format, ##__VA_ARGS__)
#define error(format, ...) do {_log("ERROR", format, ##__VA_ARGS__); abort();} while (false)
#define fatal(format, ...) do {_log("FATAL", format, ##__VA_ARGS__); abort();} while (false)

#define DISALLOW_COPY_AND_ASSIGN(T) \
	T(const T&) = delete;    \
	T& operator=(const T&) = delete;

#ifdef NDEBUG
#undef debug
#define debug(format, ...) (int)0
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
#include <thread>
#include <sstream>
#include <mutex>

using namespace std;
using rapidjson::Document;
using rapidjson::Writer;
using rapidjson::StringBuffer;

extern FILE *logfile;

#include "MagicCubeServer.h"