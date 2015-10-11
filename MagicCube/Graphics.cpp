#include "stdafx.h"
#include "Graphics.h"

#ifdef USE_GL

GLFWwindow *window;

#ifndef NO_VERTICES_BUFFER

GLuint vertexArrayId;

void initVertexArray()
{
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);
}

#endif

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