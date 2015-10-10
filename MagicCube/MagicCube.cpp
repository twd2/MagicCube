﻿// MagicCube.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MagicCube.h"

Cube cube;

#ifdef USE_GL
void graphicMode(int argc, char *argv[])
{
	//Initialize the library
	if (!glfwInit())
		throw "glfwInit";

	//MSAA
	glfwWindowHint(GLFW_SAMPLES, 9);

	//Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(WIDTH, HEIGHT, "Magic Cube", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw "glfwCreateWindow";
	}

	//Make the window's context current
	glfwMakeContextCurrent(window);

	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetCharCallback(window, characterCallback);

	initGL();

#ifndef NO_VERTICES_BUFFER
	initVertexArray();
	initAxisVertexBuffer();
	initCubeVertexBuffer();
	initCubeEdgeVertexBuffer();
#endif

	//Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		nextFrame();

		render();

		//Swap front and back buffers
		glfwSwapBuffers(window);

		//Poll for and process events
		glfwPollEvents();

		keyboardScan();
		mouseMove();
		updateFPS();
	}

	glfwTerminate();
}
#else
void textMode(int argc, char *argv[])
{
	string data;
	getline(cin, data);
	cube.Load(data);
	CubeSolver *solver = (CubeSolver*)new GeneralSolver(cube);
	solver->Solve();
	auto steps = StepReduce::Reduce(solver->Step);
	delete solver;
	printf("%s\n", stepsToString(steps).c_str());
}
#endif //USE_GL

string stepsToString(vector<CubeRotateMethod> &steps)
{
	string r;
	ptrdiff_t size = steps.size();
	for (ptrdiff_t i = 0; i < size; ++i)
	{
		r += CubeRotateMethodName[steps[i]] + " ";
	}
	return r;
}

void printError(CubeError err)
{
	if (err.why == "")
	{
		fprintf(stderr, "CubeError\n");
	}
	else
	{
		fprintf(stderr, "CubeError: %s\n", err.why.c_str());
	}
}

void printError(SolverError err)
{
	if (err.why == "")
	{
		fprintf(stderr, "SolverError\n");
	}
	else
	{
		fprintf(stderr, "SolverError: %s\n", err.why.c_str());
	}
}

int main(int argc, char *argv[])
{
	srand(clock());
#ifdef USE_GL
	graphicMode(argc, argv);
#else
	try
	{
		textMode(argc, argv);
	}
	catch (SolverError err)
	{
		printError(err);
	}
	catch (CubeError err)
	{
		printError(err);
	}
#endif //USE_GL
	return 0;
}

