#pragma once

#include "MagicCube.h"

#ifdef USE_GL

extern GLFWwindow *window;


#ifndef NO_VERTICES_BUFFER
extern GLuint vertexArrayId;
void initVertexArray();
#endif

void initGL();
void updateFPS();

#endif