#pragma once

#include "utilities.h"
#include "Cube.h"
#include "CubeSolver.h"
#include "GeneralSolver.h"
#include "StepReduce.h"

#ifdef USE_GL
#include "CommandHandlers.h"
#include "Input.h"
#include "RotationAnimation.h"
#include "Vertices.h"
#include "Graphics.h"
#include "Rendering.h"
#endif

extern Cube cube;

#ifdef USE_GL
void graphicMode(int, char *[]);
#else
void textMode(int, char *[]);
#endif //USE_GL

string stepsToString(vector<CubeRotateMethod>&);