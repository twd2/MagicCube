#pragma once

#include <vector>
#include "utilities.h"
#include "types.h"
#include "StepFilter.h"

class ReduceFilter :
	public StepFilter
{
public:
	virtual ~ReduceFilter() = 0;

	static CubeSteps Filter(CubeSteps&);

private:

	ReduceFilter();

	static bool ReduceContinuous(CubeSteps&);
	static bool ReduceInverse(CubeSteps&);
};

