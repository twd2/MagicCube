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
#endif

#ifndef USE_GL
int textMode(int, char *[]);
#endif //USE_GL

int main(int, char *[]);