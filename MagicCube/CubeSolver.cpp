#include "stdafx.h"
#include "CubeSolver.h"

string currentSolver = "general";

CubeSolver::~CubeSolver()
{
}

void CubeSolver::Do(CubeRotateMethod method)
{
	cube.DoMethod(method);
	Steps.push_back(method);
}
