#pragma once

#include "utilities.h"
#include "CubeSolver.h"

class RandomSolver : 
	public CubeSolver
{
public:
	RandomSolver(Cube &cube) : CubeSolver(cube)
	{}
	~RandomSolver();

	void Solve();
};

