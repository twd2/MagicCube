#include "stdafx.h"
#include "CubeSolver.h"

string currentSolver = "general";

CubeSolver::~CubeSolver()
{
}

void CubeSolver::Do(CubeRotateMethod method)
{
	cube.DoMethod(method);
	Step.push_back(method);
}
