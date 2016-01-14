#pragma once

#ifdef _WIN32
#endif

#ifdef __linux
#endif

#if defined(__APPLE__) && defined(__MACH__)
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
using namespace std;

#include "Config.h"

#ifdef USE_GL

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#endif

#include "../CubeCommon/CubeCommon.h"
#include "utilities.h"
