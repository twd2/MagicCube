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
	printf("Steps(%llu): %s\n", (unsigned long long)solver->Step.size(), stepsToString(solver->Step, ' ').c_str());
	CubeSteps steps = ReduceFilter::Filter(solver->Step);
	printf("Reduced steps(%llu): %s\n", (unsigned long long)steps.size(), stepsToString(steps, ' ').c_str());
	steps = NoXYZFilter::Filter(steps);
	printf("No XYZ steps(%llu): %s\n", (unsigned long long)steps.size(), stepsToString(steps, ' ').c_str());
	steps = ReduceFilter::Filter(steps);
	printf("Reduced again steps(%llu): %s\n", (unsigned long long)steps.size(), stepsToString(steps, ' ').c_str());
	delete solver;
}

void playHandler(string value)
{
	Cube oldCube = cube;
	CubeSolver *solver = (CubeSolver*)new GeneralSolver(cube);
	solver->Solve();
	printf("Steps(%llu): %s\n", (unsigned long long)solver->Step.size(), stepsToString(solver->Step, ' ').c_str());
	CubeSteps steps = ReduceFilter::Filter(solver->Step);
	printf("Reduced steps(%llu): %s\n", (unsigned long long)steps.size(), stepsToString(steps, ' ').c_str());
	steps = NoXYZFilter::Filter(steps);
	printf("No XYZ steps(%llu): %s\n", (unsigned long long)steps.size(), stepsToString(steps, ' ').c_str());
	steps = ReduceFilter::Filter(steps);
	printf("Reduced again steps(%llu): %s\n", (unsigned long long)steps.size(), stepsToString(steps, ' ').c_str());
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
			cube.Deserialize(line);
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
		if (toLowerString(value) == "cmd")
		{
			printf(">");
			string line;
			getline(cin, line);
			printf("Loading: %s\n", line.c_str());
			cube.Deserialize(line);
		}
		else if (toLowerString(value) == "format2")
		{
			printf(">");

			char f2[54];
			for (int i = 0; i < 54; ++i)
			{
				cin >> f2[i];
			}

			string data = convertFromFormat2(f2);
			printf("Loading: %s\n", data.c_str());
			cube.Deserialize(data);
		}
		else
		{
			printf("Loading: %s\n", value.c_str());
			cube.Deserialize(value);
		}
	}
	catch (const CubeError &err)
	{
		printf("CubeError: %s\n", err.what.c_str());
	}
}

void saveHandler(string value)
{
	string data = cube.Serialize();
	if (toLowerString(value) != "format2")
	{
		printf("%s\n", data.c_str());
	}
	else
	{
		char *f2 = convertToFormat2(data);
		for (int i = 0; i < 54; ++i)
		{
			printf("%c ", f2[i]);

			if ((i + 1) % 3 == 0)
			{
				printf("\n");
			}
		}
		delete[] f2;
	}
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
			printf("ERROR %s %s\n", err.what.c_str(), cube1.Serialize().c_str());
		}
		delete solver;

		if (!cube2.Check())
		{
			printf("FAIL %s\n", cube1.Serialize().c_str());
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