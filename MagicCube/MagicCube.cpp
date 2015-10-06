﻿// MagicCube.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MagicCube.h"

Cube cube;

int main(int argc, char *argv[])
{
	srand(clock());
	//srand(clock());
	for (int i = 0; i < rand() % 1000 + 1; ++i)
	{
		CubeRotateMethod method = (CubeRotateMethod)((rand() % 12) + 1);
		cube.DoMethod(method);
	}
	/*cube.Load("-WG--O-R---B-R-G-Y-YO----O-----G-W---BO-W--B--W--B-OY-"
			  "--Y--B-----W---R-W--B------------G----Y-R-----Y----OB-"
			  "O-Y--GG----YR--G-WR-G---R-----O--W--W-B-R-G---O-B--RY-");*/
	//cube.Load("-GO--W-G---W-G-R-W-OG----G-----O-B---RG-Y--G--Y--B-OY---O--W-----W---R-W--O------------R----Y-R-----Y----BY-B-O--WB----WB--R-WB-R---B-----G--R--O-G-Y-Y---O-B--RY-");
	//cube.Load("-BR--W-B---W-B-O-W-BR----B-----B-O---GY-R--Y--G--O-GY---R--W-----W---O-W--R------------O----B-Y-----Y----RY-G-R--WG----WG--O-WG-R---G-----G--O--Y-B-O-Y---O-R--YB-");
	//cube.Load("-OB--W-O---W-O-G-W-OB----O-----O-G---GO-Y--O--Y--G-RY---B--W-----W---G-W--B------------G----G-Y-----Y----RY-R-B--WR----WR--G-WR-B---R-----R--G--R-B-Y-B---Y-O--BY-");
	//cube.Load("-BR--W-B---W-B-O-W-BR----B-----B-O---BR-Y--R--Y--B-OY---R--W-----W---O-W--R------------O----O-Y-----Y----BY-G-R--WG----WG--O-WG-R---G-----G--O--G-R-Y-G---Y-G--OY-");
	//cube.Load("-OB--W-O---W-O-G-W-OB----O-----O-G---OB-Y--O--Y--O-GY---B--W-----W---G-W--B------------G----B-Y-----Y----GY-R-B--WR----WR--G-WR-B---R-----R--G--R-B-Y-R---Y-R--GY-");
	//cube.Load("-OB--W-O---W-O-G-W-OB----O-----O-G---OB-Y--R--Y--O-GY---B--W-----W---G-W--B------------G----B-Y-----Y----OY-R-B--WR----WR--G-WR-B---R-----R--G--R-B-Y-G---Y-R--GY-");
	Cube oldCube = cube;
	CubeSolver *solver = (CubeSolver*)new AlgorithmSolver(cube);
	/*try
	{*/
	solver->Solve();
	auto steps = solver->Step;

	/*}
	catch (...)
	{

	}*/
	delete solver;

	cube = oldCube;

#ifdef USE_GL
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

#ifndef NO_VERTICES_BUFFER
	initAxisVertexBuffer();
	initCubeVertexBuffer();
#endif

	play(steps);

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
#else

#endif
	return 0;
}

