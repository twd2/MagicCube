#pragma once

#include "Config.h"

#ifdef NDEBUG
#undef MEM_DEBUG
#endif

#ifdef _WIN32
#define __perror(s) fprintf(stderr, "%s: Win32 Error %d(0x%08x)\n", s, GetLastError(), GetLastError())
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

#define _log(type, format, ...) do {FILE *__fd = stdout; printTime(__fd); fprintf(__fd, "[%s] ", type); fprintf(__fd, format, ##__VA_ARGS__); fprintf(__fd, "\n");} while (false)
#define log_normal(format, ...) _log("normal", format, ##__VA_ARGS__)
#define log_debug(format, ...) _log("debug", format, ##__VA_ARGS__)
#define log_warn(format, ...) _log("WARN", format, ##__VA_ARGS__)
#define log_error(format, ...) do {_log("ERROR", format, ##__VA_ARGS__); abort();} while (false)
#define log_fatal(format, ...) do {_log("FATAL", format, ##__VA_ARGS__); abort();} while (false)

#ifdef NDEBUG
#undef log_debug
#define log_debug(format, ...) (int)0
#endif

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>

using namespace std;

#ifdef USE_GL

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <event2/event.h>
#include <event2/thread.h>
#include <event2/bufferevent.h>

using rapidjson::Document;
using rapidjson::Value;
using rapidjson::Writer;
using rapidjson::StringBuffer;

#include "../NetworkCommon/NetworkCommon.h"

#else
#define NONET
#endif

#include "utilities.h"
