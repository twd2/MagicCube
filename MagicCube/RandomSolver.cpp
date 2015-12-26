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
		CubeRotateMethod method = (CubeRotateMethod)((rand() % 19) + 1);
		if (isWholeRotate(method) || method == ROTATE_NONE || method == ROTATE_NONEi)
		{
			continue;
		}
		Do(method);
	}
}