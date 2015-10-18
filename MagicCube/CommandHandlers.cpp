#include "stdafx.h"
#include "CommandHandlers.h"

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
	printf("Steps: %s\n", stepsToString(solver->Step).c_str());
	auto steps = StepReduce::Reduce(solver->Step);
	printf("Reduced steps: %s\n", stepsToString(steps).c_str());
	delete solver;
}

void playHandler(string value)
{
	Cube oldCube = cube;
	CubeSolver *solver = (CubeSolver*)new GeneralSolver(cube);
	solver->Solve();
	printf("Steps: %s\n", stepsToString(solver->Step).c_str());
	auto steps = StepReduce::Reduce(solver->Step);
	delete solver;
	cube = oldCube;
	printf("Reduced steps: %s\n", stepsToString(steps).c_str());
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
			printf("%ld\n", count);
		}
		cube = Cube();
		for (int i = 0; i < rand() % 1000 + 1; ++i)
		{
			CubeRotateMethod method = (CubeRotateMethod)((rand() % 12) + 1);
			cube.DoMethod(method);
		}

		CubeSolver *solver = (CubeSolver*)new GeneralSolver(cube);
		try
		{
			solver->Solve();
		}
		catch (const SolverError &err)
		{
			printf("ERROR %s %s\n", err.what.c_str(), cube.Save().c_str());
		}
		delete solver;

		if (!cube.Check())
		{
			printf("FAIL %s\n", cube.Save().c_str());
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