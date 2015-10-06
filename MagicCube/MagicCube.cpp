// MagicCube.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MagicCube.h"

Cube cube;

int main(int argc, char *argv[])
{
	srand(clock());

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
	glfwSetCharCallback(window, characterCallback);

	initGL();
	
#ifndef NO_VERTICES_BUFFER
	initVertexArray();
	initAxisVertexBuffer();
	initCubeVertexBuffer();
#endif

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

