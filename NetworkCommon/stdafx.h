#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
#endif

#ifdef __linux
#endif

#if defined(__APPLE__) && defined(__MACH__)
#endif

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(T) \
	T(const T&) = delete;    \
	T& operator=(const T&) = delete;
#endif

#include <string>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>

using std::string;
using std::map;
using std::unique_lock;
using std::thread;
using std::mutex;
using std::condition_variable;
