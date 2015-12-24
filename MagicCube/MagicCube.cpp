// MagicCube.cpp : 定义控制台应用程序的入口点。
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
	
	initCommandHandlers();
	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetCharCallback(window, characterCallback);
		
	initGL();

#ifndef NO_VERTICES_BUFFER
	initVertexArray();
	initAxisVertexBuffer();
	initCubeVertexBuffer();
	initCubeEdgeVertexBuffer();
#endif

	glfwSwapInterval(1);

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
	char f2[FORMAT2_LENGTH];
	for (int i = 0; i < FORMAT2_LENGTH; ++i)
	{
		cin >> f2[i];
	}

	string data = convertFromFormat2(f2);
	cube.Deserialize(data);
	
	CubeSolver *solver = (CubeSolver*)new GeneralSolver(cube);
	solver->Solve();
	
	auto steps = ReduceFilter::Filter(solver->Step);
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
}
#endif //USE_GL

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
	catch (const SolverError &err)
	{
		printError(err);
	}
	catch (const CubeError &err)
	{
		printError(err);
	}
#endif //USE_GL
	return 0;
}

