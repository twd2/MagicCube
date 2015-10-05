#pragma once

#include "MagicCube.h"
#include "Cube.h"

extern double rotateAngle, finishAngle;
extern cube_rotate_method rotateMethod;

double easingDelta(double);
void rotateFinishCallback();
void nextAngle();
void finishCurrentRotate();
void startRotate(cube_rotate_method, bool);