#include "stdafx.h"
#include "Rendering.h"

GLfloat viewRotationAngleX, viewRotationAngleY;

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

	glPopMatrix();
}

void renderCube(Cube &cube)
{
	glPushMatrix();
	glTranslatef(-1.65f, -1.65f, -1.65f);
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

void renderCube(Cube &cube, float angle, cube_surface surface)
{
	renderSubCube(1, 1, 1, cube.subCubes[1][1][1]); //这个永远不会转动

	switch (surface)
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
	default:
		renderCubeRange(cube, 0, 2, 0, 2, 0, 2);
		break;
	}
}

void render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClearColor(0.5 + 0.5*sin(clock() / 1000.0), 0.5 + 0.5*sin(clock() / 1000.0 + PI / 3.0), 0.5 + 0.5*sin(clock() / 1000.0 + 2.0 * PI / 3.0), 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -10.0f);

	glPushMatrix();
	glRotatef(viewRotationAngleX, 1.0f, 0.0f, 0.0f); //1.0f, -1.0f, 0.0f);

	glPushMatrix();
	glRotatef(viewRotationAngleY, 0.0f, 1.0f, 0.0f); //1.0f, -1.0f, 0.0f);

	glPushMatrix();
	glScalef(3, 3, 3);

	renderAxis();

	glPopMatrix();

	renderCube(cube, (GLfloat)rotateAngle, rotateSurface);

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glFlush();
}
