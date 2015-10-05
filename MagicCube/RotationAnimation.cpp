#include "stdafx.h"
#include "RotationAnimation.h"

double rotateAngle = 0.0;
double finishAngle = 90.0;
CubeRotateMethod rotateMethod = NONE;

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
	switch (rotateMethod)
	{
	case NONE:
		break;
	case FRONT:
		cube.F();
		break;
	case BACK:
		cube.B();
		break;
	case LEFT:
		cube.L();
		break;
	case RIGHT:
		cube.R();
		break;
	case UP:
		cube.U();
		break;
	case DOWN:
		cube.D();
		break;
	case WHOLEX:
		cube.RotateUp();
		break;
	case WHOLEY:
		cube.RotateLeft();
		break;
	case WHOLEZ:
		cube.RotateClk();
		break;
	case FRONTi:
		cube.Fi();
		break;
	case BACKi:
		cube.Bi();
		break;
	case LEFTi:
		cube.Li();
		break;
	case RIGHTi:
		cube.Ri();
		break;
	case UPi:
		cube.Ui();
		break;
	case DOWNi:
		cube.Di();
		break;
	case WHOLEXi:
		cube.RotateDown();
		break;
	case WHOLEYi:
		cube.RotateRight();
		break;
	case WHOLEZi:
		cube.RotateCClk();
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
		rotateAngle += delta;
		if (rotateAngle >= finishAngle)
		{
			rotateFinishCallback();
		}
	}

	lastTime = currTime;
}

void finishCurrentRotate()
{
	rotateAngle = finishAngle;
	rotateFinishCallback();
}

void startRotate(CubeRotateMethod method)
{
	finishCurrentRotate();
	rotateMethod = method;
}