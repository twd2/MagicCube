#include "stdafx.h"
#include "RandomSolver.h"

RandomSolver::~RandomSolver()
{
}

void RandomSolver::Solve()
{
	srand(clock());
	while (!cube.Check())
	{
		CubeRotateMethod method = (CubeRotateMethod)((rand() % 12) + 1);
		cube.DoMethod(method);
		Step.push_back(method);
	}
}