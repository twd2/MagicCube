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
		startRotate(RIGHT);
	}
	else if (s == "RI")
	{
		startRotate(RIGHTi);
	}
	else if (s == "L")
	{
		startRotate(LEFT);
	}
	else if (s == "LI")
	{
		startRotate(LEFTi);
	}
	else if (s == "B")
	{
		startRotate(BACK);
	}
	else if (s == "BI")
	{
		startRotate(BACKi);
	}
	else if (s == "D")
	{
		startRotate(DOWN);
	}
	else if (s == "DI")
	{
		startRotate(DOWNi);
	}
	else if (s == "F")
	{
		startRotate(FRONT);
	}
	else if (s == "FI")
	{
		startRotate(FRONTi);
	}
	else if (s == "U")
	{
		startRotate(UP);
	}
	else if (s == "UI")
	{
		startRotate(UPi);
	}
	else if (s == "X")
	{
		startRotate(WHOLEX);
	}
	else if (s == "Y")
	{
		startRotate(WHOLEY);
	}
	else if (s == "Z")
	{
		startRotate(WHOLEZ);
	}
	else if (s == "XI")
	{
		startRotate(WHOLEXi);
	}
	else if (s == "YI")
	{
		startRotate(WHOLEYi);
	}
	else if (s == "ZI")
	{
		startRotate(WHOLEZi);
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