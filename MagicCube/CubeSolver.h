#pragma once

#include <vector>
#include "Cube.h"
#include "SolverError.h"

class CubeSolver
{
public:

	Cube &cube;
	vector<CubeRotateMethod> Step;

	CubeSolver(Cube &cube): cube(cube)
	{}

	virtual ~CubeSolver() = 0;

	virtual void Solve() = 0;

	void Do(CubeRotateMethod);
};
