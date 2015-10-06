#pragma once

#include "MagicCube.h"
#include "Cube.h"

#ifdef USE_GL

extern double rotateAngle, finishAngle;
extern CubeRotateMethod rotateMethod;

double easingDelta(double);
void rotateFinishCallback();
void nextAngle();
void finishCurrentRotate();
void startRotate(CubeRotateMethod);

#endif