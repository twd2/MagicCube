#include "stdafx.h"
#include "CubeSolver.h"

CubeSolver::~CubeSolver()
{
}

void CubeSolver::Do(CubeRotateMethod method)
{
	cube.DoMethod(method);
	Step.push_back(method);
}