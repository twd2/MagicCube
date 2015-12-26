#include "stdafx.h"
#include "BruteForceSolver.h"

BruteForceSolver::~BruteForceSolver()
{
}

void BruteForceSolver::Solve()
{
	BruteTask task0;
	task0.cube = cube;
	Q.push(task0);

	while (!Q.empty())
	{
		BruteTask task = Q.front();
		Q.pop();

		if (task.cube.Check())
		{
			Steps = task.Steps;
			break;
		}

		if (task.Steps.size() >= BRUTEFORCE_MAXSTEPS)
			continue;

		for (CubeRotateMethod method = ROTATE_FRONT; method <= ROTATE_DOWNi; method = (CubeRotateMethod)((int)method + 1))
		{
			if (isWholeRotate(method) || method == ROTATE_NONE)
				continue;

			BruteTask newTask;
			newTask.cube = task.cube;
			newTask.Steps = task.Steps;

			newTask.cube.DoMethod(method);
			newTask.Steps.push_back(method);
		
			if (!isCheckedOrInQueue[newTask.cube.Serialize()])
			{
				Q.push(newTask);
				isCheckedOrInQueue[newTask.cube.Serialize()] = true;
			}
		}
	}
}