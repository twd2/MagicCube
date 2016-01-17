#include "stdafx.h"
#include "Graphics.h"

#ifdef USE_GL

Cube cube;
GLFWwindow *window;
thread *consoleHandlerThread = NULL;
bool consoleRunning = true;

#ifndef NO_VERTICES_BUFFER

GLuint vertexArrayId;

void initVertexArray()
{
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);
}

#endif

void consoleHandler()
{
	printf("Hello, you can type commands here!\n");
	while (consoleRunning && cin)
	{
		string cmd;
		printf(">");
		getline(cin, cmd);
		double start = glfwGetTime();
		execCommand(cmd);
		printf("\nDone(%.4f ms).\n", (glfwGetTime() - start) * 1000);
	}
}

int graphicMode(int argc, char *argv[])
{
	initCommandHandlers();
	consoleHandlerThread = new thread(consoleHandler);

	//Initialize the library
	if (!glfwInit())
		throw "error glfwInit";

	//MSAA
	glfwWindowHint(GLFW_SAMPLES, 9);

	//Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(WIDTH, HEIGHT, "Magic Cube", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw "error glfwCreateWindow";
	}

	//Make the window's context current
	glfwMakeContextCurrent(window);

	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	
	// We process commands in console handler thread.
	//glfwSetKeyCallback(window, keyboardCallback);
	//glfwSetCharCallback(window, characterCallback);

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

	consoleRunning = false;

	return 0;
}

void initGL()
{
	// init OpenGL
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glPointSize(8);
	glLineWidth(2);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points  
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Antialias the lines 
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)(WIDTH) / (float)(HEIGHT), 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void updateFPS()
{
	static double lastTime = glfwGetTime();
	static int lastFPS = 0;
	double currTime = glfwGetTime();
	++lastFPS;
	if (currTime - lastTime >= 1.0)
	{
		char str[256];
#ifdef _WIN32
		sprintf_s(str, "Magic Cube (FPS: %d)", lastFPS);
#else
		snprintf(str, sizeof(str), "Magic Cube (FPS: %d)", lastFPS);
#endif
		glfwSetWindowTitle(window, str);
		lastTime = glfwGetTime();
		lastFPS = 0;
	}
}

#endif