#include "stdafx.h"
#include "Input.h"

#ifdef USE_GL

double lastX = 0.0, lastY = 0.0;
bool mouseDown = false;

double lastAngleX, lastAngleY;

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		lastAngleX = viewRotationAngleX;
		lastAngleY = viewRotationAngleY;
		glfwGetCursorPos(window, &lastX, &lastY);
		//printf("p%f, %f\n", lastX, lastY);
		mouseDown = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		mouseDown = false;
		glfwGetCursorPos(window, &lastX, &lastX);
	}
}

void mouseMove()
{
	if (!mouseDown) return;

	double currX, currY;
	glfwGetCursorPos(window, &currX, &currY);

	double deltaX = currX - lastX,
		deltaY = currY - lastY;

	//printf("d%f, %f\n", deltaX, deltaY);

	viewRotationAngleY = (GLfloat)(lastAngleY + deltaX*0.5);
	viewRotationAngleX = (GLfloat)(lastAngleX + deltaY*0.5);
}

void execCommand(string s)
{
	if (s == "R")
	{
		startRotate(ROTATE_RIGHT);
	}
	else if (s == "RI")
	{
		startRotate(ROTATE_RIGHTi);
	}
	else if (s == "L")
	{
		startRotate(ROTATE_LEFT);
	}
	else if (s == "LI")
	{
		startRotate(ROTATE_LEFTi);
	}
	else if (s == "B")
	{
		startRotate(ROTATE_BACK);
	}
	else if (s == "BI")
	{
		startRotate(ROTATE_BACKi);
	}
	else if (s == "D")
	{
		startRotate(ROTATE_DOWN);
	}
	else if (s == "DI")
	{
		startRotate(ROTATE_DOWNi);
	}
	else if (s == "F")
	{
		startRotate(ROTATE_FRONT);
	}
	else if (s == "FI")
	{
		startRotate(ROTATE_FRONTi);
	}
	else if (s == "U")
	{
		startRotate(ROTATE_UP);
	}
	else if (s == "UI")
	{
		startRotate(ROTATE_UPi);
	}
	else if (s == "X")
	{
		startRotate(ROTATE_WHOLEX);
	}
	else if (s == "Y")
	{
		startRotate(ROTATE_WHOLEY);
	}
	else if (s == "Z")
	{
		startRotate(ROTATE_WHOLEZ);
	}
	else if (s == "XI")
	{
		startRotate(ROTATE_WHOLEXi);
	}
	else if (s == "YI")
	{
		startRotate(ROTATE_WHOLEYi);
	}
	else if (s == "ZI")
	{
		startRotate(ROTATE_WHOLEZi);
	}
	else if (s == "CHECK")
	{
		printf("U%d D%d L%d R%d F%d B%d: %d\n", cube.CheckU(), cube.CheckD(), cube.CheckL(), cube.CheckR(), cube.CheckF(), cube.CheckB(), cube.Check());
	}
	else if (s == "ABOUT")
	{
		printf("Wandai :)\n");
	}
	else if (s == "SOLVE")
	{
		CubeSolver *solver = (CubeSolver*)new AlgorithmSolver(cube);
		solver->Solve();
		delete solver;
	}
	else if (s == "TEST")
	{
		unsigned long long count = 0;
		for (;;)
		{
			++count;
			if (count % 10000 == 0)
			{
				printf("%ld\n", count);
			}
			cube = Cube();
			for (int i = 0; i < rand() % 1000 + 1; ++i)
			{
				CubeRotateMethod method = (CubeRotateMethod)((rand() % 12) + 1);
				cube.DoMethod(method);
			}

			CubeSolver *solver = (CubeSolver*)new AlgorithmSolver(cube);
			try
			{
				solver->Solve();
			}
			catch (SolverError err)
			{
				printf("ERROR%s %s\n", err.why.c_str(), cube.Save().c_str());
			}
			delete solver;

			if (!cube.Check())
			{
				printf("FAIL¡¡%s\n", cube.Save().c_str());
			}
		}
	}
}

string commandBuffer = "";

void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		if (key >= GLFW_KEY_SPACE && key <= GLFW_KEY_GRAVE_ACCENT)
		{
			commandBuffer += (char)key;
			printf("%c", key);
		}
		else if (key == GLFW_KEY_BACKSPACE)
		{
			if (commandBuffer.length() > 0)
			{
				commandBuffer = commandBuffer.substr(0, commandBuffer.length() - 1);
			}
			printf("\r%s ", commandBuffer.c_str());
			printf("\r%s", commandBuffer.c_str());
		}
		else if (key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER)
		{
			printf("\n");
			execCommand(commandBuffer);
			printf("\nDone.\n");
			commandBuffer = "";
		}
	}
}

void keyboardScan()
{
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		viewRotationAngleY -= 0.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		viewRotationAngleY += 0.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		viewRotationAngleX -= 0.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		viewRotationAngleX += 0.5f;
	}
}

#endif