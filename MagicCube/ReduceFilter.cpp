#include "stdafx.h"
#include "ReduceFilter.h"


ReduceFilter::~ReduceFilter()
{
}

CubeSteps ReduceFilter::Filter(CubeSteps &steps)
{
	CubeSteps newSteps = steps;
	while (ReduceContinuous(newSteps) ||
		ReduceInverse(newSteps))
	{
		CubeSteps newNewSteps;
		copySteps(newSteps, newNewSteps);
		newSteps = newNewSteps;
	}
	return newSteps;
}

bool ReduceFilter::ReduceContinuous(CubeSteps &steps)
{
	size_t size = steps.size();
	bool found = false;
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size - 2; ++i)
	{
		if (steps[i] == steps[i + 1] && steps[i] == steps[i + 2])
		{
			steps[i] = inverse(steps[i]);
			steps[i + 1] = steps[i + 2] = ROTATE_NONE;
			found = true;
		}
	}
	return found;
}

bool ReduceFilter::ReduceInverse(CubeSteps &steps)
{
	size_t size = steps.size();
	bool found = false;
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size - 1; ++i)
	{
		if (steps[i] == inverse(steps[i + 1]))
		{
			steps[i] = steps[i + 1] = ROTATE_NONE;
			found = true;
		}
	}
	return found;
}