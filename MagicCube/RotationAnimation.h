#pragma once

#include "MagicCube.h"
#include "Cube.h"

extern double rotateAngle;
extern cube_surface rotateSurface;

double easingDelta(double);
void rotateFinishCallback();
void nextAngle();
void finishCurrentRotate();
void startRotate(cube_surface, bool);