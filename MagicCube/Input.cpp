#include "stdafx.h"
#include "Input.h"

#ifdef USE_GL

double lastX = 0.0, lastY = 0.0;
bool mouseDown = false;

double lastAngleX, lastAngleY;

map<string, ptrCommandHandler> commandHandler;

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		lastAngleX = viewRotationAngleX;
		lastAngleY = viewRotationAngleY;
		glfwGetCursorPos(window, &lastX, &lastY);
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

	viewRotationAngleY = (GLfloat)(lastAngleY + deltaX*0.5);
	viewRotationAngleX = (GLfloat)(lastAngleX + deltaY*0.5);
}

void addCommandHandler(string cmd, ptrCommandHandler handler)
{
	commandHandler[cmd] = handler;
}

void execCommand(string cmd)
{
	try
	{
		string value = "";
		size_t index = cmd.find(' ');
		if (index != string::npos)
		{
			value = cmd.substr(index + 1);
			cmd = cmd.substr(0, index);
		}
		cmd = toUpperString(cmd);

		// other commands
		ptrCommandHandler handler = commandHandler[cmd];
		if (handler != NULL)
		{
			handler(value);
		}
		else
		{
			// rotate
			handler = commandHandler["ROTATE"];
			if (handler != NULL)
			{
				handler(cmd);
			}
		}
	}
	catch (const SolverError &err)
	{
		printError(err);
	}
	catch (const CubeError &err)
	{
		printError(err);
	}
}

string commandBuffer = "";

void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		if (key == GLFW_KEY_BACKSPACE)
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
			double start = glfwGetTime();
			execCommand(commandBuffer);
			printf("\nDone(%.4f ms).\n", (glfwGetTime() - start) * 1000);
			commandBuffer = "";
		}
	}
}

void characterCallback(GLFWwindow *window, unsigned int codepoint)
{
	if (codepoint < 256)
	{
		char ch = (char)codepoint;
		commandBuffer += ch;
		printf("%c", ch);
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