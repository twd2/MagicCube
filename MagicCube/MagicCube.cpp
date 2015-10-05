// MagicCube.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MagicCube.h"

Cube cube;

int main(int argc, char *argv[])
{
	cube.Load("-WG--O-R---B-R-G-Y-YO----O-----G-W---BO-W--B--W--B-OY-"
		      "--Y--B-----W---R-W--B------------G----Y-R-----Y----OB-"
			  "O-Y--GG----YR--G-WR-G---R-----O--W--W-B-R-G---O-B--RY-");

	CubeSolver *solver = (CubeSolver*)new AlgorithmSolver(cube);
	solver->Solve();
	delete solver;

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

	initGL();
	initVertexArray();
	initAxisVertexBuffer();
	initCubeVertexBuffer();

	//Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		nextAngle();

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
	return 0;
}

