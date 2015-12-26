#pragma once

#include "utilities.h"
#include "Cube.h"
#include "CubeSolver.h"
#include "GeneralSolver.h"
#include "ReduceFilter.h"
#include "NoXYZFilter.h"

#ifdef USE_GL
#include "CommandHandlers.h"
#include "Input.h"
#include "RotationAnimation.h"
#include "Vertices.h"
#include "Graphics.h"
#include "Rendering.h"

extern Cube cube;
#endif

#ifdef USE_GL
void graphicMode(int, char *[]);
#else
void textMode(int, char *[]);
#endif //USE_GL
