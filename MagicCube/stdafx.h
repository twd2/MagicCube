// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#ifdef _WIN32
#include "targetver.h"
#endif

#ifdef linux
#endif

#ifdef _UNIX
#endif

#include <stdio.h>
#include <cmath>
#include <ctime>

#include "types.h"

#include <string>
#include <iostream>

using namespace std;

#include <GL\glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "Config.h"

#define min(a,b) (((a)<(b))?(a):(b))

// TODO:  在此处引用程序需要的其他头文件
