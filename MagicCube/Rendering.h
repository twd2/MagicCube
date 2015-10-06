#pragma once

#include "MagicCube.h"
#include "Graphics.h"
#include "Vertices.h"
#include "RotationAnimation.h"
#include "Cube.h"

#ifdef USE_GL

extern GLfloat viewRotationAngleX, viewRotationAngleY;

void renderAxis();
void renderSubCube(GLfloat, GLfloat, GLfloat, cube_t);
void renderCube(Cube&);
inline void renderCubeRange(Cube&, int, int, int, int, int, int);
void renderCube(Cube&, float, CubeRotateMethod);
void render();

#endif