#pragma once

#include <vector>
#include "types.h"

class StepReduce
{
public:
	StepReduce();
	~StepReduce();

	static vector<CubeRotateMethod> Reduce(vector<CubeRotateMethod>&);

private:
	static void Copy(vector<CubeRotateMethod>&, vector<CubeRotateMethod>&);
};

