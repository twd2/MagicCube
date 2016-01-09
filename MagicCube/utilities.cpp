#include "stdafx.h"
#include "utilities.h"

void printError(SolverError err)
{
	if (err.what == "")
	{
		fprintf(stderr, "SolverError\n");
	}
	else
	{
		fprintf(stderr, "SolverError: %s\n", err.what.c_str());
	}
}

CubeSolver *newSolver(Cube &cube)
{
	if (currentSolver == "general")
	{
		return (CubeSolver*)new GeneralSolver(cube);
	}
	else if (currentSolver == "random")
	{
		return (CubeSolver*)new RandomSolver(cube);
	}
	else if (currentSolver == "bruteforce")
	{
		return (CubeSolver*)new BruteForceSolver(cube);
	}
	else
	{
		throw "Unknown solver";
	}
}

CubeSteps solveAndPrint(Cube cube)
{
	printf("Solving...\n");

	CubeSolver *solver = newSolver(cube);
	solver->Solve();
	CubeSteps steps = solver->Steps;
	delete solver;

	printf("Steps(%llu): %s\n", (unsigned long long)steps.size(), stepsToString(steps, ' ').c_str());
	steps = ReduceFilter::Filter(steps);
	printf("Reduced steps(%llu): %s\n", (unsigned long long)steps.size(), stepsToString(steps, ' ').c_str());
	steps = NoXYZFilter::Filter(steps);
	printf("No XYZ steps(%llu): %s\n", (unsigned long long)steps.size(), stepsToString(steps, ' ').c_str());
	steps = ReduceFilter::Filter(steps);
	printf("Reduced again steps(%llu): %s\n", (unsigned long long)steps.size(), stepsToString(steps, ' ').c_str());

	return steps;
}