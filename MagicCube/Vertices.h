#pragma once

#ifdef USE_GL

extern const GLfloat axisVertexBufferData[];
extern const GLfloat cubeVertexBufferData[];

#ifndef NO_VERTICES_BUFFER
extern GLuint axisVertexBuffer, cubeVertexBuffer;
void initAxisVertexBuffer();
void initCubeVertexBuffer();
#endif

#endif