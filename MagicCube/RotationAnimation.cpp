#include "stdafx.h"
#include "RotationAnimation.h"

double rotateAngle = 0.0;
cube_surface rotateSurface = NONE;

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

	switch (rotateSurface)
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
	default:
		break;
	}

	rotateAngle = 0.0;
	rotateSurface = NONE;
}

void nextAngle()
{
	static double lastTime = glfwGetTime();
	double currTime = glfwGetTime();

	if (rotateSurface != NONE)
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
		if (abs(rotateAngle) >= 90.0)
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
		rotateAngle = -90.0;
	}
	else
	{
		rotateAngle = 90.0;
	}
	rotateFinishCallback();
}

void startRotate(cube_surface surface, bool inverse)
{
	finishCurrentRotate();
	rotateSurface = surface;
	if (inverse)
	{
		rotateAngle = -0.01;
	}
}