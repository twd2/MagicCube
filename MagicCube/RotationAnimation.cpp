#include "stdafx.h"
#include "RotationAnimation.h"

double rotateAngle = 0.0;
double finishAngle = 90.0;
cube_rotate_method rotateMethod = NONE;

double easingDelta(double currentAngle)
{
	currentAngle = abs(currentAngle);
	if (currentAngle == 0.0)
	{
		return 180.0;
	}
	else
	{
		return min(180.0, 8100.0 / currentAngle);
	}
}

void rotateFinishCallback()
{
	bool inverse = rotateAngle < 0.0;

	switch (rotateMethod)
	{
	case NONE:
		break;
	case FRONT:
		if (inverse)
		{
			cube.Fi();
		}
		else
		{
			cube.F();
		}
		break;
	case BACK:
		if (inverse)
		{
			cube.Bi();
		}
		else
		{
			cube.B();
		}
		break;
	case LEFT:
		if (inverse)
		{
			cube.Li();
		}
		else
		{
			cube.L();
		}
		break;
	case RIGHT:
		if (inverse)
		{
			cube.Ri();
		}
		else
		{
			cube.R();
		}
		break;
	case UP:
		if (inverse)
		{
			cube.Ui();
		}
		else
		{
			cube.U();
		}
		break;
	case DOWN:
		if (inverse)
		{
			cube.Di();
		}
		else
		{
			cube.D();
		}
		break;
	case WHOLEX:
		if (inverse)
		{
			cube.RotateDown();
		}
		else
		{
			cube.RotateUp();
		}
		break;
	case WHOLEY:
		if (inverse)
		{
			cube.RotateRight();
		}
		else
		{
			cube.RotateLeft();
		}
		break;
	case WHOLEZ:
		if (inverse)
		{
			cube.RotateCClk();
		}
		else
		{
			cube.RotateClk();
		}
		break;
	default:
		break;
	}

	rotateAngle = 0.0;
	rotateMethod = NONE;
}

void nextAngle()
{
	static double lastTime = glfwGetTime();
	double currTime = glfwGetTime();

	if (rotateMethod != NONE)
	{
		double delta = easingDelta(rotateAngle) * (currTime - lastTime);
		if (rotateAngle < 0.0)
		{
			rotateAngle -= delta;
		}
		else
		{
			rotateAngle += delta;
		}
		if (abs(rotateAngle) >= finishAngle)
		{
			rotateFinishCallback();
		}
	}

	lastTime = currTime;
}

void finishCurrentRotate()
{
	if (rotateAngle < 0.0)
	{
		rotateAngle = -finishAngle;
	}
	else
	{
		rotateAngle = finishAngle;
	}
	rotateFinishCallback();
}

void startRotate(cube_rotate_method method, bool inverse)
{
	finishCurrentRotate();
	rotateMethod = method;
	if (inverse)
	{
		rotateAngle = -0.01;
	}
}