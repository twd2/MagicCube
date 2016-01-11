#pragma once

#include "Config.h"

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
#define __perror(s) printf("%s: Win32 Error %d(0x%08x)\n", s, GetLastError(), GetLastError())
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
#define _perror(s) {__perror(s); /*abort();*/}
#else
#define _perror(s) __perror(s)
#endif

#if (!defined(ENABLE_IPV4) && !defined(ENABLE_IPV6))
#error Cannot disable ipv4 and ipv6 at the same time.
#endif

#define _log(type, format, ...) {printTime(stdout); printf("[%s] ", type); printf(format, __VA_ARGS__); printf("\n");}
#define normal(format, ...) _log("normal", format, __VA_ARGS__)
#define debug(format, ...) _log("debug", format, __VA_ARGS__)
#define error(format, ...) {_log("ERROR", format, __VA_ARGS__); abort();}
#define fatal(format, ...) {_log("FATAL", format, __VA_ARGS__); abort();}

#define SYNC_LOCK(mtx) { unique_lock<mutex> lck(mtx);
#define END_SYNC_LOCK }

#define DISALLOW_COPY_AND_ASSIGN(T) \
	T(const T&) = delete;    \
	T& operator=(const T&) = delete;

#ifdef NDEBUG
#undef debug
#define debug(format, ...)
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

#include "MagicCubeServer.h"