#pragma once

#include <vector>
#include "../CubeCommon/CubeCommon.h"
#include "SolverError.h"

class CubeSolver
{
public:

	Cube &cube;
	vector<CubeRotateMethod> Steps;

	CubeSolver(Cube &cube): cube(cube)
	{}

	virtual ~CubeSolver() = 0;

	virtual void Solve() = 0;

	void Do(CubeRotateMethod);
};

extern string currentSolver;