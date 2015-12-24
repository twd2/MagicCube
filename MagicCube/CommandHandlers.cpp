#include "stdafx.h"
#include "CommandHandlers.h"

#ifdef USE_GL

void checkHandler(string value)
{
	printf("U%d D%d L%d R%d F%d B%d: %d\n", cube.CheckU(), cube.CheckD(), cube.CheckL(), cube.CheckR(), cube.CheckF(), cube.CheckB(), cube.Check());
}

void aboutHandler(string value)
{
	printf("Wandai :)\n");
}

void solveHandler(string value)
{
	CubeSolver *solver = (CubeSolver*)new GeneralSolver(cube);
	solver->Solve();
	printf("Steps(%llu): %s\n", solver->Step.size(), stepsToString(solver->Step).c_str());
	CubeSteps steps = ReduceFilter::Filter(solver->Step);
	printf("Reduced steps(%llu): %s\n", steps.size(), stepsToString(steps).c_str());
	steps = NoXYZFilter::Filter(steps);
	printf("No XYZ steps(%llu): %s\n", steps.size(), stepsToString(steps).c_str());
	steps = ReduceFilter::Filter(steps);
	printf("Reduced again steps(%llu): %s\n", steps.size(), stepsToString(steps).c_str());
	delete solver;
}

void playHandler(string value)
{
	Cube oldCube = cube;
	CubeSolver *solver = (CubeSolver*)new GeneralSolver(cube);
	solver->Solve();
	printf("Steps(%llu): %s\n", solver->Step.size(), stepsToString(solver->Step).c_str());
	CubeSteps steps = ReduceFilter::Filter(solver->Step);
	printf("Reduced steps(%llu): %s\n", steps.size(), stepsToString(steps).c_str());
	steps = NoXYZFilter::Filter(steps);
	printf("No XYZ steps(%llu): %s\n", steps.size(), stepsToString(steps).c_str());
	steps = ReduceFilter::Filter(steps);
	printf("Reduced again steps(%llu): %s\n", steps.size(), stepsToString(steps).c_str());
	delete solver;
	cube = oldCube;
	play(steps);
}

void randomHandler(string value)
{
	randomCube(cube);
}

void fileHandler(string value)
{
	try
	{
		ifstream file(value);
		if (file.is_open())
		{
			string line;
			getline(file, line);
			printf("Loading: %s\n", line.c_str());
			cube.Load(line);
			file.close();
		}
		else
		{
			printf("ERROR\n");
		}
	}
	catch (const CubeError &err)
	{
		printf("CubeError: %s\n", err.what.c_str());
	}
}

void loadHandler(string value)
{
	try
	{
		if (toUpperString(value) == "CMD")
		{
			printf(">");
			string line;
			getline(cin, line);
			printf("Loading: %s\n", line.c_str());
			cube.Load(line);
		}
		else
		{
			printf("Loading: %s\n", value.c_str());
			cube.Load(value);
		}
	}
	catch (const CubeError &err)
	{
		printf("CubeError: %s\n", err.what.c_str());
	}
}

void saveHandler(string value)
{
	string data = cube.Save();
	printf("%s\n", data.c_str());
}

void testHandler(string value)
{
	unsigned long long count = 0;
	for (;;)
	{
		++count;
		if (count % 10000 == 0)
		{
			printf("%llu\n", count);
		}
		
		Cube cube1, cube2;

		for (int i = 0; i < rand() % 1000 + 1; ++i)
		{
			CubeRotateMethod method = (CubeRotateMethod)((rand() % 12) + 1);
			cube1.DoMethod(method);
		}

		cube2 = cube1;

		CubeSolver *solver = (CubeSolver*)new GeneralSolver(cube1);
		try
		{
			solver->Solve();
			CubeSteps steps = solver->Step;
			steps = ReduceFilter::Filter(steps);
			steps = NoXYZFilter::Filter(steps);
			steps = ReduceFilter::Filter(steps);
			for (auto step : steps)
			{
				cube2.DoMethod(step);
			}
		}
		catch (const SolverError &err)
		{
			printf("ERROR %s %s\n", err.what.c_str(), cube1.Save().c_str());
		}
		delete solver;

		if (!cube2.Check())
		{
			printf("FAIL %s\n", cube1.Save().c_str());
		}
	}
}

void tranHandler(string value)
{
	isTransparent = !isTransparent;
}

void echoHandler(string value)
{
	printf("%s\n", value.c_str());
}

void initCommandHandlers()
{
	addCommandHandler("CHECK", checkHandler);
	addCommandHandler("ABOUT", aboutHandler);
	addCommandHandler("SOLVE", solveHandler);
	addCommandHandler("PLAY", playHandler);
	addCommandHandler("RANDOM", randomHandler);
	addCommandHandler("FILE", fileHandler);
	addCommandHandler("LOAD", loadHandler);
	addCommandHandler("SAVE", saveHandler);
	addCommandHandler("TEST", testHandler);
	addCommandHandler("TRAN", tranHandler);
	addCommandHandler("ECHO", echoHandler);
}

#endif