#pragma once

#include <vector>
#include "utilities.h"
#include "types.h"

class StepReduce
{
public:
	StepReduce();
	~StepReduce();

	static vector<CubeRotateMethod> Reduce(vector<CubeRotateMethod>&);

private:
	static void Copy(vector<CubeRotateMethod>&, vector<CubeRotateMethod>&);
	static bool ReduceContinuous(vector<CubeRotateMethod>&);
	static bool ReduceI(vector<CubeRotateMethod>&);
};

