#include "stdafx.h"
#include "Rendering.h"

#ifdef USE_GL

GLfloat viewRotationAngleX = 45.0, viewRotationAngleY = -45.0;

void renderAxis()
{
#ifndef NO_VERTICES_BUFFER
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

	//x
	glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
	glDrawArrays(GL_LINES, 0, 3 * 2);

	//y
	glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
	glDrawArrays(GL_LINES, 3 * 2, 3 * 2);

	//z
	glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
	glDrawArrays(GL_LINES, 6 * 2, 3 * 2);

	glDisableVertexAttribArray(0);
#else
	//x
	glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
	glBegin(GL_LINES);
	for (int i = 0; i < 6; ++i)
	{
		glVertex3f(axisVertexBufferData[i * 3 + 0],
			axisVertexBufferData[i * 3 + 1],
			axisVertexBufferData[i * 3 + 2]);
	}
	glEnd();

	//y
	glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
	glBegin(GL_LINES);
	for (int i = 6; i < 12; ++i)
	{
		glVertex3f(axisVertexBufferData[i * 3 + 0],
			axisVertexBufferData[i * 3 + 1],
			axisVertexBufferData[i * 3 + 2]);
	}
	glEnd();

	//z
	glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
	glBegin(GL_LINES);
	for (int i = 12; i < 18; ++i)
	{
		glVertex3f(axisVertexBufferData[i * 3 + 0],
			axisVertexBufferData[i * 3 + 1],
			axisVertexBufferData[i * 3 + 2]);
	}
	glEnd();
#endif
}

void renderSubCube(GLfloat x, GLfloat y, GLfloat z, cube_t colorInfo)
{
	glPushMatrix();
	glTranslatef(-1.6f, -1.6f, -1.6f);

	glPushMatrix();
	glTranslatef(1.1f * x, 1.1f * y, 1.1f * z);

#ifndef NO_VERTICES_BUFFER
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
	
	setColor((CubeColor)GET_FRONT(colorInfo));
	glDrawArrays(GL_TRIANGLES, 0, 2 * 3);

	setColor((CubeColor)GET_BACK(colorInfo));
	glDrawArrays(GL_TRIANGLES, 2 * 3, 2 * 3);

	setColor((CubeColor)GET_LEFT(colorInfo));
	glDrawArrays(GL_TRIANGLES, 2 * 2 * 3, 2 * 3);

	setColor((CubeColor)GET_RIGHT(colorInfo));
	glDrawArrays(GL_TRIANGLES, 3 * 2 * 3, 2 * 3);

	setColor((CubeColor)GET_UP(colorInfo));
	glDrawArrays(GL_TRIANGLES, 4 * 2 * 3, 2 * 3);

	setColor((CubeColor)GET_DOWN(colorInfo));
	glDrawArrays(GL_TRIANGLES, 5 * 2 * 3, 2 * 3);

	glDisableVertexAttribArray(0);
#else
	setColor((CubeColor)GET_FRONT(colorInfo));
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 6; ++i)
	{
		glVertex3f(cubeVertexBufferData[i * 3 + 0],
			cubeVertexBufferData[i * 3 + 1],
			cubeVertexBufferData[i * 3 + 2]);
	}
	glEnd();

	setColor((CubeColor)GET_BACK(colorInfo));
	glBegin(GL_TRIANGLES);
	for (int i = 6; i < 12; ++i)
	{
		glVertex3f(cubeVertexBufferData[i * 3 + 0],
			cubeVertexBufferData[i * 3 + 1],
			cubeVertexBufferData[i * 3 + 2]);
	}
	glEnd();

	setColor((CubeColor)GET_LEFT(colorInfo));
	glBegin(GL_TRIANGLES);
	for (int i = 12; i < 18; ++i)
	{
		glVertex3f(cubeVertexBufferData[i * 3 + 0],
			cubeVertexBufferData[i * 3 + 1],
			cubeVertexBufferData[i * 3 + 2]);
	}
	glEnd();

	setColor((CubeColor)GET_RIGHT(colorInfo));
	glBegin(GL_TRIANGLES);
	for (int i = 18; i < 24; ++i)
	{
		glVertex3f(cubeVertexBufferData[i * 3 + 0],
			cubeVertexBufferData[i * 3 + 1],
			cubeVertexBufferData[i * 3 + 2]);
	}
	glEnd();

	setColor((CubeColor)GET_UP(colorInfo));
	glBegin(GL_TRIANGLES);
	for (int i = 24; i < 30; ++i)
	{
		glVertex3f(cubeVertexBufferData[i * 3 + 0],
			cubeVertexBufferData[i * 3 + 1],
			cubeVertexBufferData[i * 3 + 2]);
	}
	glEnd();

	setColor((CubeColor)GET_DOWN(colorInfo));
	glBegin(GL_TRIANGLES);
	for (int i = 30; i < 36; ++i)
	{
		glVertex3f(cubeVertexBufferData[i * 3 + 0],
			cubeVertexBufferData[i * 3 + 1],
			cubeVertexBufferData[i * 3 + 2]);
	}
	glEnd();

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < 24; ++i)
	{
		glVertex3f(cubeEdgeVertexBufferData[i * 3 + 0],
			cubeEdgeVertexBufferData[i * 3 + 1],
			cubeEdgeVertexBufferData[i * 3 + 2]);
	}
	glEnd();
#endif

	glPopMatrix();

	glPopMatrix();
}

void renderCube(Cube &cube)
{
	glPushMatrix();
	glTranslatef(-1.6f, -1.6f, -1.6f);
	for (int x = 0; x <= 2; ++x)
	{
		for (int y = 0; y <= 2; ++y)
		{
			for (int z = 0; z <= 2; ++z)
			{
				renderSubCube((GLfloat)x, (GLfloat)y, (GLfloat)z, cube.subCubes[x][y][z]);
			}
		}
	}
	glPopMatrix();
}

inline void renderCubeRange(Cube &cube, int x0, int x1, int y0, int y1, int z0, int z1)
{
	for (int x = x0; x <= x1; ++x)
	{
		for (int y = y0; y <= y1; ++y)
		{
			for (int z = z0; z <= z1; ++z)
			{
				renderSubCube((GLfloat)x, (GLfloat)y, (GLfloat)z, cube.subCubes[x][y][z]);
			}
		}
	}
}

void renderCube(Cube &cube, float angle, CubeRotateMethod method)
{
	renderSubCube(1, 1, 1, cube.subCubes[1][1][1]); //never rotate

	switch (method)
	{
	case ROTATE_FRONT:
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, -1.0);
		renderCubeRange(cube, 0, 2, 0, 2, 2, 2);
		glPopMatrix();
		renderCubeRange(cube, 0, 2, 0, 2, 0, 1);
		break;
	case ROTATE_BACK:
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		renderCubeRange(cube, 0, 2, 0, 2, 0, 0);
		glPopMatrix();
		renderCubeRange(cube, 0, 2, 0, 2, 1, 2);
		break;
	case ROTATE_LEFT:
		glPushMatrix();
		glRotatef(angle, 1.0, 0.0, 0.0);
		renderCubeRange(cube, 0, 0, 0, 2, 0, 2);
		glPopMatrix();
		renderCubeRange(cube, 1, 2, 0, 2, 0, 2);
		break;
	case ROTATE_RIGHT:
		glPushMatrix();
		glRotatef(angle, -1.0, 0.0, 0.0);
		renderCubeRange(cube, 2, 2, 0, 2, 0, 2);
		glPopMatrix();
		renderCubeRange(cube, 0, 1, 0, 2, 0, 2);
		break;
	case ROTATE_UP:
		glPushMatrix();
		glRotatef(angle, 0.0, -1.0, 0.0);
		renderCubeRange(cube, 0, 2, 2, 2, 0, 2);
		glPopMatrix();
		renderCubeRange(cube, 0, 2, 0, 1, 0, 2);
		break;
	case ROTATE_DOWN:
		glPushMatrix();
		glRotatef(angle, 0.0, 1.0, 0.0);
		renderCubeRange(cube, 0, 2, 0, 0, 0, 2);
		glPopMatrix();
		renderCubeRange(cube, 0, 2, 1, 2, 0, 2);
		break;
	case ROTATE_WHOLEX:
		glPushMatrix();
		glRotatef(angle, -1.0, 0.0, 0.0);
		renderCubeRange(cube, 0, 2, 0, 2, 0, 2);
		glPopMatrix();
		break;
	case ROTATE_WHOLEY:
		glPushMatrix();
		glRotatef(angle, 0.0, -1.0, 0.0);
		renderCubeRange(cube, 0, 2, 0, 2, 0, 2);
		glPopMatrix();
		break;
	case ROTATE_WHOLEZ:
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, -1.0);
		renderCubeRange(cube, 0, 2, 0, 2, 0, 2);
		glPopMatrix();
		break;
	case ROTATE_FRONTi:
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		renderCubeRange(cube, 0, 2, 0, 2, 2, 2);
		glPopMatrix();
		renderCubeRange(cube, 0, 2, 0, 2, 0, 1);
		break;
	case ROTATE_BACKi:
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, -1.0);
		renderCubeRange(cube, 0, 2, 0, 2, 0, 0);
		glPopMatrix();
		renderCubeRange(cube, 0, 2, 0, 2, 1, 2);
		break;
	case ROTATE_LEFTi:
		glPushMatrix();
		glRotatef(angle, -1.0, 0.0, 0.0);
		renderCubeRange(cube, 0, 0, 0, 2, 0, 2);
		glPopMatrix();
		renderCubeRange(cube, 1, 2, 0, 2, 0, 2);
		break;
	case ROTATE_RIGHTi:
		glPushMatrix();
		glRotatef(angle, 1.0, 0.0, 0.0);
		renderCubeRange(cube, 2, 2, 0, 2, 0, 2);
		glPopMatrix();
		renderCubeRange(cube, 0, 1, 0, 2, 0, 2);
		break;
	case ROTATE_UPi:
		glPushMatrix();
		glRotatef(angle, 0.0, 1.0, 0.0);
		renderCubeRange(cube, 0, 2, 2, 2, 0, 2);
		glPopMatrix();
		renderCubeRange(cube, 0, 2, 0, 1, 0, 2);
		break;
	case ROTATE_DOWNi:
		glPushMatrix();
		glRotatef(angle, 0.0, -1.0, 0.0);
		renderCubeRange(cube, 0, 2, 0, 0, 0, 2);
		glPopMatrix();
		renderCubeRange(cube, 0, 2, 1, 2, 0, 2);
		break;
	case ROTATE_WHOLEXi:
		glPushMatrix();
		glRotatef(angle, 1.0, 0.0, 0.0);
		renderCubeRange(cube, 0, 2, 0, 2, 0, 2);
		glPopMatrix();
		break;
	case ROTATE_WHOLEYi:
		glPushMatrix();
		glRotatef(angle, 0.0, 1.0, 0.0);
		renderCubeRange(cube, 0, 2, 0, 2, 0, 2);
		glPopMatrix();
		break;
	case ROTATE_WHOLEZi:
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		renderCubeRange(cube, 0, 2, 0, 2, 0, 2);
		glPopMatrix();
		break;
	default:
		renderCubeRange(cube, 0, 2, 0, 2, 0, 2);
		break;
	}
}

void render()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -10.0f);

	glPushMatrix();
	glRotatef(viewRotationAngleX, 1.0f, 0.0f, 0.0f);

	glPushMatrix();
	glRotatef(viewRotationAngleY, 0.0f, 1.0f, 0.0f);

	glPushMatrix();
	glScalef(3, 3, 3);

	renderAxis();

	glPopMatrix();

	renderCube(cube, (GLfloat)rotateAngle, rotateMethod);

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glFlush();
}

#endif