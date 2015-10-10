#pragma once
#include "CubeSolver.h"
class BruteForceSolver :
	public CubeSolver
{
public:
	BruteForceSolver(Cube& cube) : CubeSolver(cube)
	{}
	~BruteForceSolver();

	void Solve();
};

