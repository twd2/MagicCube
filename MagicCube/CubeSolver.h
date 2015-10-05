#pragma once

#include <vector>
#include "Cube.h"

class CubeSolver
{
public:

	Cube &cube;
	vector<CubeRotateMethod> Step;

	CubeSolver(Cube& cube): cube(cube)
	{}

	~CubeSolver();

	virtual void Solve() = 0;

	void Do(CubeRotateMethod);
};
