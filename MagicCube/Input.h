#pragma once

#include "MagicCube.h"
#include "Graphics.h"
#include "Rendering.h"

extern double lastX, lastY, lastAngleX, lastAngleY;
extern bool mouseDown;

void mouseButtonCallback(GLFWwindow*, int, int, int);
void mouseMove();
void execCommand(string);
void keyboardCallback(GLFWwindow*, int, int, int, int);
