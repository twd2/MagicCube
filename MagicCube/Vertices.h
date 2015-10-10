#pragma once

#ifdef USE_GL

extern const GLfloat axisVertexBufferData[];
extern const GLfloat cubeVertexBufferData[];
extern const GLfloat cubeEdgeVertexBufferData[];

#ifndef NO_VERTICES_BUFFER
extern GLuint axisVertexBuffer, cubeVertexBuffer, cubeEdgeVertexBuffer;
void initAxisVertexBuffer();
void initCubeVertexBuffer();
void initCubeEdgeVertexBuffer();
#endif

#endif