#pragma once

#include "../CubeCommon/CubeCommon.h"
#include "utilities.h"
#include "Input.h"
#include "NoXYZFilter.h"
#include "RandomSolver.h"
#include "BruteForceSolver.h"

#ifdef USE_GL

typedef void(*CommandHandler)(string);

void initCommandHandlers();
void addCommandHandler(string, CommandHandler);
void execCommand(string);

extern map<string, CommandHandler> commandHandler;

#endif