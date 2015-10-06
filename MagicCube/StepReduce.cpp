#include "stdafx.h"
#include "StepReduce.h"


StepReduce::StepReduce()
{
}


StepReduce::~StepReduce()
{
}

vector<CubeRotateMethod> StepReduce::Reduce(vector<CubeRotateMethod>& steps)
{
	vector<CubeRotateMethod> newSteps = steps;
	while (ReduceContinuous(newSteps) ||
		ReduceI(newSteps))
	{
		vector<CubeRotateMethod> newNewSteps;
		Copy(newSteps, newNewSteps);
		newSteps = newNewSteps;
	}
	return newSteps;
}

void StepReduce::Copy(vector<CubeRotateMethod>& src, vector<CubeRotateMethod>& dest)
{
	size_t size = src.size();
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size; ++i)
	{
		if (src[i] != ROTATE_NONE && src[i] != ROTATE_NONE2)
			dest.push_back(src[i]);
	}
}

bool StepReduce::ReduceContinuous(vector<CubeRotateMethod>& steps)
{
	size_t size = steps.size();
	bool found = false;
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size - 2; ++i)
	{
		if (steps[i] == steps[i + 1] && steps[i] == steps[i + 2])
		{
			if (steps[i] < ROTATE_NONE2)
			{
				steps[i] = (CubeRotateMethod)(steps[i] + 10); //inverse
			}
			else
			{
				steps[i] = (CubeRotateMethod)(steps[i] - 10); //inverse
			}
			steps[i + 1] = steps[i + 2] = ROTATE_NONE;
			found = true;
		}
	}
	return found;
}

bool StepReduce::ReduceI(vector<CubeRotateMethod>& steps)
{
	size_t size = steps.size();
	bool found = false;
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size - 1; ++i)
	{
		if (steps[i] == steps[i + 1] + 10 || steps[i] + 10 == steps[i + 1]) // == inverse
		{
			steps[i] = steps[i + 1] = ROTATE_NONE;
			found = true;
		}
	}
	return found;
}