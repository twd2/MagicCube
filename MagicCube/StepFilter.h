#pragma once
class StepFilter
{
public:
	virtual ~StepFilter() = 0;

	static CubeSteps Filter(CubeSteps&);

private:
	StepFilter();
};

