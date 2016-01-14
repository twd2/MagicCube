#pragma once

#include "../CubeCommon/CubeCommon.h"
#include "utilities.h"
#include "Input.h"
#include "NoXYZFilter.h"
#include "RandomSolver.h"
#include "BruteForceSolver.h"

#ifdef USE_GL

typedef void(*ptrCommandHandler)(string);

void initCommandHandlers();
void addCommandHandler(string, ptrCommandHandler);
void execCommand(string);

extern map<string, ptrCommandHandler> commandHandler;

#endif