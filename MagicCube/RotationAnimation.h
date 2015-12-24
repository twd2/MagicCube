#pragma once

#include "MagicCube.h"
#include "Cube.h"

#ifdef USE_GL

extern bool playing;
extern CubeSteps stepsToPlay;
extern ptrdiff_t playIndex;

extern double rotateAngle, finishAngle;
extern CubeRotateMethod rotateMethod;

extern const double speed;

double easingDelta(double);
void rotateFinishCallback();
void nextFrame();
void finishCurrentRotate();
void startRotate(CubeRotateMethod);
void play(CubeSteps&);
void playNext();
void stopPlay();

#endif