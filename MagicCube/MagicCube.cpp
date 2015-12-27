// MagicCube.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MagicCube.h"

#ifndef USE_GL
int textMode(int argc, char *argv[])
{
	Cube cube;

	char f2[FORMAT2_LENGTH];
	for (int i = 0; i < FORMAT2_LENGTH; ++i)
	{
		cin >> f2[i];
	}

	string data = convertFromFormat2(f2);
	cube.Deserialize(data);
	
	CubeSolver *solver = newSolver(cube);
	solver->Solve();
	
	auto steps = ReduceFilter::Filter(solver->Steps);
	steps = NoXYZFilter::Filter(steps);
	steps = ReduceFilter::Filter(steps);
	
	delete solver;

	if (steps.size() > 0)
	{
		printf("%s", stepsToString(steps, '\n').c_str());
	}
	else
	{
		printf("Nothing to do.\n");
	}
	return 0;
}
#endif //USE_GL

int main(int argc, char *argv[])
{
	srand(clock());
#ifdef USE_GL
	try
	{
		return graphicMode(argc, argv);
	}
	catch (const string &err)
	{
		printf("%s\n", err.c_str());
		return 1;
	}
#else
	try
	{
		return textMode(argc, argv);
	}
	catch (const SolverError &err)
	{
		printError(err);
		return 1;
	}
	catch (const CubeError &err)
	{
		printError(err);
		return 1;
	}
#endif //USE_GL
	return 0;
}

