#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
#endif

#ifdef __linux
#endif

#if defined(__APPLE__) && defined(__MACH__)
#endif

#include <vector>
#include <map>
#include <string>
#include <sstream>

using std::vector;
using std::map;
using std::string;
using std::stringstream;
using std::ptrdiff_t;

#include "CubeTypes.h"
#include "utilities.h"
