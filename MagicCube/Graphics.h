#pragma once

#include "MagicCube.h"

#ifdef USE_GL

extern Cube cube;
extern GLFWwindow *window;

#ifndef NO_VERTICES_BUFFER
extern GLuint vertexArrayId;
void initVertexArray();
#endif

int graphicMode(int, char*[]);
void initGL();
void updateFPS();

#endif