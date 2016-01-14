#include "stdafx.h"
#include "CommandHandlers.h"

#ifdef USE_GL

map<string, CommandHandler> commandHandler;

void rotateHandler(string value)
{
	string rotateCmd = toUpperString(value);
	if (rotateCmd.length() > 1)
		rotateCmd[1] = toLower(rotateCmd[1]);

	CubeRotateMethod method = NameToCubeRotateMethod[rotateCmd];
	if (method != ROTATE_NONE)
	{
		startRotate(method);
	}
	else
	{
		printf("Unknown command %s\n", value.c_str());
	}
}

void checkHandler(string value)
{
	printf("U%d D%d L%d R%d F%d B%d: %d\n", cube.CheckU(), cube.CheckD(), cube.CheckL(), cube.CheckR(), cube.CheckF(), cube.CheckB(), cube.Check());
}

void helpHandler(string value)
{
	printf(":)\n");
}

void aboutHandler(string value)
{
	printf("Wandai :)\n");
}

void resetHandler(string value)
{
	cube = Cube();
}

void solveHandler(string value)
{
	solveAndPrint(cube);
}

void playHandler(string value)
{
	CubeSteps steps = solveAndPrint(cube);
	play(steps);
}

void stopHandler(string value)
{
	stopPlay();
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
		if (toLowerString(value) == "")
		{
			printf("Format1>");
			string line;
			getline(cin, line);
			printf("Loading: %s\n", line.c_str());
			cube.Deserialize(line);
		}
		else if (toLowerString(value) == "format2")
		{
			printf("Format2>");

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
			CubeSteps steps = solver->Steps;
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

void axisHandler(string value)
{
	doRenderAxis = !doRenderAxis;
}

void echoHandler(string value)
{
	printf("%s\n", value.c_str());
}

void set_solverHandler(string value)
{
	value = toLowerString(value);
	if (value == "" || value == "general")
	{
		currentSolver = "general";
	}
	else if (value == "random" || value == "monkey")
	{
		currentSolver = "random";
	}
	else if (value == "bruteforce" || value == "bf")
	{
		currentSolver = "bruteforce";
	}
}

void get_solverHandler(string value)
{
	printf("%s\n", currentSolver.c_str());
}


void addCommandHandler(string cmd, CommandHandler handler)
{
	commandHandler[cmd] = handler;
}

void execCommand(string cmd)
{
	try
	{
		string value = "";
		size_t index = cmd.find(' ');
		if (index != string::npos)
		{
			value = cmd.substr(index + 1);
			cmd = cmd.substr(0, index);
		}
		cmd = toLowerString(cmd);

		// other commands
		CommandHandler handler = commandHandler[cmd];
		if (handler != NULL)
		{
			handler(value);
		}
		else
		{
			// rotate
			handler = commandHandler["rotate"];
			if (handler != NULL)
			{
				handler(cmd);
			}
			else
			{
				printf("Unknown command %s\n", cmd.c_str());
			}
		}
	}
	catch (const SolverError &err)
	{
		printError(err);
	}
	catch (const CubeError &err)
	{
		printError(err);
	}
}

void initCommandHandlers()
{
#define HAND(x) addCommandHandler(#x, x##Handler)
	HAND(rotate);
	HAND(check);
	HAND(help);
	HAND(about);
	HAND(reset);
	HAND(solve);
	HAND(play);
	HAND(stop);
	HAND(random);
	HAND(file);
	HAND(load);
	HAND(save);
	HAND(test);
	HAND(tran);
	HAND(axis);
	HAND(echo);
	HAND(set_solver);
	HAND(get_solver);
#undef HAND
}

#endif