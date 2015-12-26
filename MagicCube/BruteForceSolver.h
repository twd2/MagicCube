#pragma once

#include "utilities.h"
#include "CubeSolver.h"
#include <queue>
#include <map>

#ifndef BRUTEFORCE_MAXSTEPS
#define BRUTEFORCE_MAXSTEPS 20
#endif

class BruteTask
{
public:
	Cube cube;
	CubeSteps Steps;
};

class BruteForceSolver :
	public CubeSolver
{
public:
	BruteForceSolver(Cube &cube) : CubeSolver(cube)
	{}
	~BruteForceSolver();

	void Solve();

private:

	queue<BruteTask> Q;
	map<string, bool> isCheckedOrInQueue;
};

