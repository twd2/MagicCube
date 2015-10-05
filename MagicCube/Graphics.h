#pragma once

#include "MagicCube.h"

extern GLFWwindow *window;
extern GLuint vertexArrayId;

void initVertexArray();
void initGL();
void setColor(CubeColor);
void updateFPS();