#include "stdafx.h"
#include "Vertices.h"

#ifdef USE_GL

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

void initAxisVertexBuffer()
{
	glGenBuffers(1, &axisVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, axisVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertexBufferData), axisVertexBufferData, GL_STATIC_DRAW);
}

void initCubeVertexBuffer()
{
	glGenBuffers(1, &cubeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexBufferData), cubeVertexBufferData, GL_STATIC_DRAW);
}

#endif