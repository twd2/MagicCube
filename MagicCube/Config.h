#pragma once

#define WIDTH  800
#define HEIGHT 600

#define USE_GL

#define NO_VERTICES_BUFFER

#ifdef NOGL
#undef USE_GL
#endif