#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "CubeError.h"
#include "SolverError.h"
#include "Cube.h"

vector<string> split(const string&, char);
char toUpper(char);
char toLower(char);
string toUpperString(string);
string toLowerString(string);
string stepsToString(CubeSteps&);
void printError(CubeError);
void printError(SolverError);
void randomCube(Cube&);
string randomCube();
CubeRotateMethod inverse(CubeRotateMethod);

// copy steps with deleting NONE steps
void copySteps(CubeSteps &src, CubeSteps &dest);

bool isWholeRotate(CubeRotateMethod);