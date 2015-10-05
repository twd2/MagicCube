#pragma once

#include "CubeSolver.h"

class AlgorithmSolver :
	public CubeSolver
{
public:
	AlgorithmSolver(Cube &cube) : CubeSolver(cube)
	{}
	~AlgorithmSolver();

	void Solve();
};

