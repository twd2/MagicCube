#include "stdafx.h"
#include "Rendering.h"

#ifdef USE_GL

GLfloat viewRotationAngleX = 45.0, viewRotationAngleY = -45.0;

void renderAxis()
{
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
}

void renderSubCube(GLfloat x, GLfloat y, GLfloat z, cube_t colorInfo)
{
	glPushMatrix();
	glTranslatef(-1.6f, -1.6f, -1.6f);
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
	case FRONT:
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, -1.0);
		renderCubeRange(cube, 0, 2, 0, 2, 2, 2);
		glPopMatrix();
		renderCubeRange(cube, 0, 2, 0, 2, 0, 1);
		break;
	case BACK:
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		renderCubeRange(cube, 0, 2, 0, 2, 0, 0);
		glPopMatrix();
		renderCubeRange(cube, 0, 2, 0, 2, 1, 2);
		break;
	case LEFT:
		glPushMatrix();
		glRotatef(angle, 1.0, 0.0, 0.0);
		renderCubeRange(cube, 0, 0, 0, 2, 0, 2);
		glPopMatrix();
		renderCubeRange(cube, 1, 2, 0, 2, 0, 2);
		break;
	case RIGHT:
		glPushMatrix();
		glRotatef(angle, -1.0, 0.0, 0.0);
		renderCubeRange(cube, 2, 2, 0, 2, 0, 2);
		glPopMatrix();
		renderCubeRange(cube, 0, 1, 0, 2, 0, 2);
		break;
	case UP:
		glPushMatrix();
		glRotatef(angle, 0.0, -1.0, 0.0);
		renderCubeRange(cube, 0, 2, 2, 2, 0, 2);
		glPopMatrix();
		renderCubeRange(cube, 0, 2, 0, 1, 0, 2);
		break;
	case DOWN:
		glPushMatrix();
		glRotatef(angle, 0.0, 1.0, 0.0);
		renderCubeRange(cube, 0, 2, 0, 0, 0, 2);
		glPopMatrix();
		renderCubeRange(cube, 0, 2, 1, 2, 0, 2);
		break;
	case WHOLEX:
		glPushMatrix();
		glRotatef(angle, -1.0, 0.0, 0.0);
		renderCubeRange(cube, 0, 2, 0, 2, 0, 2);
		glPopMatrix();
		break;
	case WHOLEY:
		glPushMatrix();
		glRotatef(angle, 0.0, -1.0, 0.0);
		renderCubeRange(cube, 0, 2, 0, 2, 0, 2);
		glPopMatrix();
		break;
	case WHOLEZ:
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, -1.0);
		renderCubeRange(cube, 0, 2, 0, 2, 0, 2);
		glPopMatrix();
		break;
	case FRONTi:
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, 1.0);
		renderCubeRange(cube, 0, 2, 0, 2, 2, 2);
		glPopMatrix();
		renderCubeRange(cube, 0, 2, 0, 2, 0, 1);
		break;
	case BACKi:
		glPushMatrix();
		glRotatef(angle, 0.0, 0.0, -1.0);
		renderCubeRange(cube, 0, 2, 0, 2, 0, 0);
		glPopMatrix();
		renderCubeRange(cube, 0, 2, 0, 2, 1, 2);
		break;
	case LEFTi:
		glPushMatrix();
		glRotatef(angle, -1.0, 0.0, 0.0);
		renderCubeRange(cube, 0, 0, 0, 2, 0, 2);
		glPopMatrix();
		renderCubeRange(cube, 1, 2, 0, 2, 0, 2);
		break;
	case RIGHTi:
		glPushMatrix();
		glRotatef(angle, 1.0, 0.0, 0.0);
		renderCubeRange(cube, 2, 2, 0, 2, 0, 2);
		glPopMatrix();
		renderCubeRange(cube, 0, 1, 0, 2, 0, 2);
		break;
	case UPi:
		glPushMatrix();
		glRotatef(angle, 0.0, 1.0, 0.0);
		renderCubeRange(cube, 0, 2, 2, 2, 0, 2);
		glPopMatrix();
		renderCubeRange(cube, 0, 2, 0, 1, 0, 2);
		break;
	case DOWNi:
		glPushMatrix();
		glRotatef(angle, 0.0, -1.0, 0.0);
		renderCubeRange(cube, 0, 2, 0, 0, 0, 2);
		glPopMatrix();
		renderCubeRange(cube, 0, 2, 1, 2, 0, 2);
		break;
	case WHOLEXi:
		glPushMatrix();
		glRotatef(angle, 1.0, 0.0, 0.0);
		renderCubeRange(cube, 0, 2, 0, 2, 0, 2);
		glPopMatrix();
		break;
	case WHOLEYi:
		glPushMatrix();
		glRotatef(angle, 0.0, 1.0, 0.0);
		renderCubeRange(cube, 0, 2, 0, 2, 0, 2);
		glPopMatrix();
		break;
	case WHOLEZi:
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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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