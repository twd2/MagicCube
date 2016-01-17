#pragma once

#include "CubeError.h"
#include "Cube.h"

string stepsToString(CubeSteps&, char);
void printError(CubeError);
void randomCube(Cube&);
string randomCube();
CubeRotateMethod inverse(CubeRotateMethod);

// copy steps with deleting NONE steps
void copySteps(CubeSteps&, CubeSteps&);

bool isWholeRotate(CubeRotateMethod);

// format2: see pair 1
string convertFromFormat2(char*);
char *convertToFormat2(string);
