#pragma once

#include "MagicCube.h"
#include "Cube.h"

#ifdef USE_GL

extern bool playing;
extern vector<CubeRotateMethod> *stepsToPlay;
extern int playIndex;

extern double rotateAngle, finishAngle;
extern CubeRotateMethod rotateMethod;

double easingDelta(double);
void rotateFinishCallback();
void nextAngle();
void finishCurrentRotate();
void startRotate(CubeRotateMethod);
void play(vector<CubeRotateMethod>&);
void playNext();
void stopPlay();

#endif