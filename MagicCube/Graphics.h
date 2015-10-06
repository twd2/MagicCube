#pragma once

#include "MagicCube.h"

#ifdef USE_GL

extern GLFWwindow *window;
extern GLuint vertexArrayId;

void initVertexArray();
void initGL();
void setColor(CubeColor);
void updateFPS();

#endif