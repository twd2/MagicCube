#include "stdafx.h"
#include "Input.h"

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
		startRotate(RIGHT, false);
	}
	else if (s == "RI")
	{
		startRotate(RIGHT, true);
	}
	else if (s == "L")
	{
		startRotate(LEFT, false);
	}
	else if (s == "LI")
	{
		startRotate(LEFT, true);
	}
	else if (s == "B")
	{
		startRotate(BACK, false);
	}
	else if (s == "BI")
	{
		startRotate(BACK, true);
	}
	else if (s == "D")
	{
		startRotate(DOWN, false);
	}
	else if (s == "DI")
	{
		startRotate(DOWN, true);
	}
	else if (s == "F")
	{
		startRotate(FRONT, false);
	}
	else if (s == "FI")
	{
		startRotate(FRONT, true);
	}
	else if (s == "U")
	{
		startRotate(UP, false);
	}
	else if (s == "UI")
	{
		startRotate(UP, true);
	}
	else if (s == "X")
	{
		startRotate(WHOLEX, false);
	}
	else if (s == "Y")
	{
		startRotate(WHOLEY, false);
	}
	else if (s == "Z")
	{
		startRotate(WHOLEZ, false);
	}
	else if (s == "XI")
	{
		startRotate(WHOLEX, true);
	}
	else if (s == "YI")
	{
		startRotate(WHOLEY, true);
	}
	else if (s == "ZI")
	{
		startRotate(WHOLEZ, true);
	}
	else if (s == "CHECK")
	{
		printf("U%d D%d L%d R%d F%d B%d: %d\n", cube.CheckU(), cube.CheckD(), cube.CheckL(), cube.CheckR(), cube.CheckF(), cube.CheckB(), cube.Check());
	}
	else if (s == "ABOUT")
	{
		printf("Wandai :)\n");
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
		else if (key == GLFW_KEY_ENTER)
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