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
		if (cmd == "R")
		{
			startRotate(ROTATE_RIGHT);
		}
		else if (cmd == "RI")
		{
			startRotate(ROTATE_RIGHTi);
		}
		else if (cmd == "L")
		{
			startRotate(ROTATE_LEFT);
		}
		else if (cmd == "LI")
		{
			startRotate(ROTATE_LEFTi);
		}
		else if (cmd == "B")
		{
			startRotate(ROTATE_BACK);
		}
		else if (cmd == "BI")
		{
			startRotate(ROTATE_BACKi);
		}
		else if (cmd == "D")
		{
			startRotate(ROTATE_DOWN);
		}
		else if (cmd == "DI")
		{
			startRotate(ROTATE_DOWNi);
		}
		else if (cmd == "F")
		{
			startRotate(ROTATE_FRONT);
		}
		else if (cmd == "FI")
		{
			startRotate(ROTATE_FRONTi);
		}
		else if (cmd == "U")
		{
			startRotate(ROTATE_UP);
		}
		else if (cmd == "UI")
		{
			startRotate(ROTATE_UPi);
		}
		else if (cmd == "X")
		{
			startRotate(ROTATE_WHOLEX);
		}
		else if (cmd == "Y")
		{
			startRotate(ROTATE_WHOLEY);
		}
		else if (cmd == "Z")
		{
			startRotate(ROTATE_WHOLEZ);
		}
		else if (cmd == "XI")
		{
			startRotate(ROTATE_WHOLEXi);
		}
		else if (cmd == "YI")
		{
			startRotate(ROTATE_WHOLEYi);
		}
		else if (cmd == "ZI")
		{
			startRotate(ROTATE_WHOLEZi);
		}
		else if (cmd == "CHECK")
		{
			printf("U%d D%d L%d R%d F%d B%d: %d\n", cube.CheckU(), cube.CheckD(), cube.CheckL(), cube.CheckR(), cube.CheckF(), cube.CheckB(), cube.Check());
		}
		else if (cmd == "ABOUT")
		{
			printf("Wandai :)\n");
		}
		else if (cmd == "SOLVE")
		{
			CubeSolver *solver = (CubeSolver*)new GeneralSolver(cube);
			solver->Solve();
			auto steps = StepReduce::Reduce(solver->Step);
			printf("Steps: %s\n", stepsToString(steps).c_str());
			delete solver;
		}
		else if (cmd == "PLAY")
		{
			Cube oldCube = cube;
			CubeSolver *solver = (CubeSolver*)new GeneralSolver(cube);
			solver->Solve();
			auto steps = StepReduce::Reduce(solver->Step);
			delete solver;
			cube = oldCube;
			printf("Steps: %s\n", stepsToString(steps).c_str());
			play(steps);
		}
		else if (cmd == "RANDOM")
		{
			randomCube(cube);
		}
		else if (cmd == "FILE")
		{
			try
			{
				ifstream file(value);
				if (file.is_open())
				{
					string line;
					getline(file, line);
					printf("Loading: %s\n", line.c_str());
					cube.Load(line);
					file.close();
				}
				else
				{
					printf("ERROR\n");
				}
			}
			catch (CubeError err)
			{
				printf("CubeError: %s\n", err.why.c_str());
			}
		}
		else if (cmd == "LOAD")
		{
			if (toUpperString(value) == "CMD")
			{
				printf(">");
				string line;
				getline(cin, line);
				printf("Loading: %s\n", line.c_str());
				cube.Load(line);
			}
			else
			{
				cube.Load(value);
			}
		}
		else if (cmd == "SAVE")
		{
			string data = cube.Save();
			printf("%s\n", data.c_str());
		}
		else if (cmd == "TEST")
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

				CubeSolver *solver = (CubeSolver*)new GeneralSolver(cube);
				try
				{
					solver->Solve();
				}
				catch (SolverError err)
				{
					printf("ERROR %s %s\n", err.why.c_str(), cube.Save().c_str());
				}
				delete solver;

				if (!cube.Check())
				{
					printf("FAIL %s\n", cube.Save().c_str());
				}
			}
		}
	}
	catch (SolverError err)
	{
		printError(err);
	}
	catch (CubeError err)
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
			execCommand(commandBuffer);
			printf("\nDone.\n");
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