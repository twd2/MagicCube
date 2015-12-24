#pragma once

#include <vector>
#include "utilities.h"
#include "types.h"
#include "StepFilter.h"

class ReduceFilter
	: StepFilter
{
public:
	ReduceFilter();
	~ReduceFilter();

	static CubeSteps Filter(CubeSteps&);

private:
	static bool ReduceContinuous(CubeSteps&);
	static bool ReduceInverse(CubeSteps&);
};

