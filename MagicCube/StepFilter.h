#pragma once

#include "utilities.h"
#include "../CubeCommon/CubeCommon.h"

class StepFilter
{
public:
	virtual ~StepFilter() = 0;

	static CubeSteps Filter(CubeSteps&);

private:
	StepFilter();
};

