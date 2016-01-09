#pragma once

#ifdef _WIN32
#include "targetver.h"
typedef int socklen_t;
#define INET6_ADDRSTRLEN 46
#endif

#ifdef linux
#endif

#ifdef _UNIX
#endif

#include "Config.h"

#include <event2/event.h>
#include <event2/bufferevent.h>

#include <iostream>
#include <map>
#include <thread>
#include <sstream>

using namespace std;
