#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "../CubeCommon/CubeCommon.h"
#include "CubeSolver.h"
#include "GeneralSolver.h"
#include "BruteForceSolver.h"
#include "RandomSolver.h"
#include "ReduceFilter.h"
#include "NoXYZFilter.h"
#include "SolverError.h"

void printError(SolverError);
void printTime(FILE*);
CubeSolver *newSolver(Cube&);
CubeSteps solveAndPrint(Cube);