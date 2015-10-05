// MagicCube.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MagicCube.h"

Cube cube;

void updateFPS()
{
	static double lastTime = glfwGetTime();
	static int lastFPS = 0;
	double currTime = glfwGetTime();
	++lastFPS;
	if (currTime - lastTime >= 1.0)
	{
		char str[256];
		sprintf_s(str, "Magic Cube (FPS: %d)", lastFPS);
		glfwSetWindowTitle(window, str);
		lastTime = glfwGetTime();
		lastFPS = 0;
	}
}

int main(int argc, char* argv[])
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_SAMPLES, 9);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Magic Cube", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyboardCallback);

	initGL();
	initVertexArray();
	initAxisVertexBuffer();
	initCubeVertexBuffer();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		nextAngle();

		/* Render here */
		render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		mouseMove();
		updateFPS();
	}

	glfwTerminate();
	return 0;
}

