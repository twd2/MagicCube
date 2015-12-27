#pragma once

#include "utilities.h"
#include "Cube.h"
#include "Input.h"
#include "NoXYZFilter.h"
#include "RandomSolver.h"
#include "BruteForceSolver.h"

#ifdef USE_GL

void initCommandHandlers();
void addCommandHandler(string, ptrCommandHandler);
void execCommand(string);

extern map<string, ptrCommandHandler> commandHandler;

#endif