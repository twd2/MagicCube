// MagicCube.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

GLFWwindow* window;

#define PI 3.14159265358979
#define WIDTH  640
#define HEIGHT 480

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
	gluPerspective(45.0f, (float)(WIDTH) / (float)(HEIGHT), 0.5f, -1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glEnable(GL_LIGHTING);
	//glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_LIGHT0);
	//GLfloat lightAmbient[] = { 0.0, 1.0, 0.0, 1.0 };
	//GLfloat lightDiffuse[] = { 0.0, 1.0, 0.0, 1.0 };
	//GLfloat lightSpecular[] = { 1.0, 0.0, 0.0, 1.0 };
	////glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	//GLfloat lightPosition[] = {0.0f, 0.1f, 0.0f, 1.0f};   // w不为0
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0);     // c 系数
	//glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);        // l 系数
	//glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5f);    // q 系数
	//glColorMaterial(GL_FRONT, GL_DIFFUSE);
	if (glewInit() != GLEW_OK)
	{
		throw "glewInit";
	}
}

GLuint vertexArrayId;

void initVertexArray()
{
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);
}

GLfloat angleX, angleY;

void Boxes()
{
	glPushMatrix();
	glTranslatef(-1.5f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glEnd();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(1.5f, 0.0f, 0.0f);

	glBegin(GL_QUADS);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	glColor4f(0.5f, 0.5f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glEnd();
	glPopMatrix();
}

inline void setColor(cube_color color)
{
	switch (color)
	{
	case COLOR_UNUSED:
		glColor4f(0.0, 0.0, 0.0, 1.0);
		break;
	case COLOR_BLUE:
		glColor4f(0.0, 0.0, 1.0, 1.0);
		break;
	case COLOR_GREEN:
		glColor4f(0.0, 1.0, 0.0, 1.0);
		break;
	case COLOR_ORANGE:
		glColor4f(1.0, 0.5, 0.0, 1.0);
		break;
	case COLOR_RED:
		glColor4f(1.0, 0.0, 0.0, 1.0);
		break;
	case COLOR_WHITE:
		glColor4f(1.0, 1.0, 1.0, 1.0);
		break;
	case COLOR_YELLOW:
		glColor4f(1.0, 1.0, 0.0, 1.0);
		break;
	default:
		break;
	}
}

int bufferId = 0;

inline int allocBuffer()
{
	return bufferId++;
}

GLuint axisVertexBuffer, cubeVertexBuffer;

const GLfloat axisVertexBufferData[] = {
	//x
	-1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.975f, 0.025f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.975f, -0.025f, 0.0f,
	1.0f, 0.0f, 0.0f,
	//y
	0.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.025f, 0.975f, 0.0f,
	0.0f, 1.0f, 0.0f,
	-0.025f, 0.975f, 0.0f,
	0.0f, 1.0f, 0.0f,
	//z
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.025f, 0.975f,
	0.0f, 0.0f, 1.0f,
	0.0f, -0.025f, 0.975f,
	0.0f, 0.0f, 1.0f
};

const GLfloat cubeVertexBufferData[] = {
	//Front
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	//Back
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	//Left
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	//Right
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	//Up
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	//Down
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

void initAxisBuffer()
{
	glGenBuffers(1, &axisVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, axisVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertexBufferData), axisVertexBufferData, GL_STATIC_DRAW);
}

void initCubeBuffer()
{
	glGenBuffers(1, &cubeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexBufferData), cubeVertexBufferData, GL_STATIC_DRAW);
}

void renderAxis()
{
	glColor4f(0.5f, 0.5f, 0.5f, 0.5f);

	glBegin(GL_POINTS);
	glVertex2f(0.0f, 0.0f);
	glEnd();

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, axisVertexBuffer);
	glVertexAttribPointer(
		0,                  // index
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
	glDrawArrays(GL_LINES, 0, 3 * 2);
	glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
	glDrawArrays(GL_LINES, 3 * 2, 3 * 2);
	glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
	glDrawArrays(GL_LINES, 6 * 2, 3 * 2);

	glDisableVertexAttribArray(0);
}

void renderSubCube(GLfloat x, GLfloat y, GLfloat z, cube_t colorInfo)
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
	glVertexAttribPointer(
		0,                  // index
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glPushMatrix();
	glTranslatef(1.1f * x, 1.1f * y, 1.1f * z);

	setColor((cube_color)GET_FRONT(colorInfo));
	glDrawArrays(GL_TRIANGLES, 0, 2 * 3);
	setColor((cube_color)GET_BACK(colorInfo));
	glDrawArrays(GL_TRIANGLES, 2 * 3, 2 * 3);
	setColor((cube_color)GET_LEFT(colorInfo));
	glDrawArrays(GL_TRIANGLES, 2 * 2 * 3, 2 * 3);
	setColor((cube_color)GET_RIGHT(colorInfo));
	glDrawArrays(GL_TRIANGLES, 3 * 2 * 3, 2 * 3);
	setColor((cube_color)GET_UP(colorInfo));
	glDrawArrays(GL_TRIANGLES, 4 * 2 * 3, 2 * 3);
	setColor((cube_color)GET_DOWN(colorInfo));
	glDrawArrays(GL_TRIANGLES, 5 * 2 * 3, 2 * 3);
	glDisableVertexAttribArray(0);

	glPopMatrix();
}

void renderCube(Cube &cube)
{
	glPushMatrix();
	glTranslatef(-1.65f, -1.65f, -1.65f);
	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int z = 0; z < 3; ++z)
			{
				renderSubCube(x, y, z, cube.subCubes[x][y][z]);
			}
		}
	}
	glPopMatrix();
}

Cube cube;

void render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClearColor(0.5 + 0.5*sin(clock() / 1000.0), 0.5 + 0.5*sin(clock() / 1000.0 + PI / 3.0), 0.5 + 0.5*sin(clock() / 1000.0 + 2.0 * PI / 3.0), 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -10.0f);

	glPushMatrix();
	glRotatef(angleX, -1.0f, 0.0f, 0.0f); //1.0f, -1.0f, 0.0f);

	glPushMatrix();
	glRotatef(angleY, 0.0f, -1.0f, 0.0f); //1.0f, -1.0f, 0.0f);

	glPushMatrix();
	//glScalef(100, 100, 100);

	renderAxis();

	glPopMatrix();

	glPushMatrix();

	renderCube(cube);

	//Boxes();
	
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glFlush();
}

double lastX = 0.0, lastY = 0.0;
bool mouseDown = false;

double lastAngleX, lastAngleY;

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		lastAngleX = angleX;
		lastAngleY = angleY;
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

void mouse_move()
{
	if (!mouseDown) return;

	double currX, currY;
	glfwGetCursorPos(window, &currX, &currY);

	double deltaX = currX - lastX,
		    deltaY = currY - lastY;
	
	//printf("d%f, %f\n", deltaX, deltaY);

	angleY = lastAngleY - deltaX*0.5;
	angleX = lastAngleX - deltaY*0.5;
}

void execCommand(string s)
{
	if (s == "R")
	{
		cube.R();
	}
	else if (s == "RI")
	{
		cube.Ri();
	}
	else if (s == "L")
	{
		cube.L();
	}
	else if (s == "LI")
	{
		cube.Li();
	}
	else if (s == "B")
	{
		cube.B();
	}
	else if (s == "BI")
	{
		cube.Bi();
	}
	else if (s == "D")
	{
		cube.D();
	}
	else if (s == "DI")
	{
		cube.Di();
	}
	else if (s == "F")
	{
		cube.F();
	}
	else if (s == "FI")
	{
		cube.Fi();
	}
	else if (s == "U")
	{
		cube.U();
	}
	else if (s == "UI")
	{
		cube.Ui();
	}
}


string commandBuffer = "";

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_LEFT)
		{
			angleY += 0.05f;
			printf("%f\n", angleY);
		}
		else if (key == GLFW_KEY_RIGHT)
		{
			angleY -= 0.05f;
			printf("%f\n", angleY);
		}
		else if (key == GLFW_KEY_UP)
		{
			angleX += 0.05f;
			printf("%f\n", angleX);
		}
		else if (key == GLFW_KEY_DOWN)
		{
			angleX -= 0.05f;
			printf("%f\n", angleX);
		}
		else if (key >= GLFW_KEY_SPACE && key <= GLFW_KEY_GRAVE_ACCENT)
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
			execCommand(commandBuffer);
			printf("Done.\n");
			commandBuffer = "";
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Magic Cube", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);

	initGL();
	initVertexArray();
	initAxisBuffer();
	initCubeBuffer();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		mouse_move();
		
		/*else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		{
			cout << "COMMAND> ";
			string s;
			getline(cin, s);
			execCommand(s);
		}*/
	}

	glfwTerminate();

	cube_t x = 0x00300510;
	printf("%x\n", ROTATE_LEFT(x));
	printf("%x\n", ROTATE_RIGHT(x));
	printf("%x\n", ROTATE_UP(x));
	printf("%x\n", ROTATE_DOWN(x));
	printf("%x\n", ROTATE_CLK(x));
	printf("%x\n", ROTATE_CCLK(x));
	//getchar();
	return 0;
}

