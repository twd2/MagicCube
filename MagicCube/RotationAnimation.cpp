#include "stdafx.h"
#include "RotationAnimation.h"

#ifdef USE_GL

bool playing = false;
vector<CubeRotateMethod> stepsToPlay;
ptrdiff_t playIndex = 0;

double rotateAngle = 0.0;
double finishAngle = 90.0;
CubeRotateMethod rotateMethod = ROTATE_NONE;

double lastPlayTime = glfwGetTime();

const double speed = 1.5;

double easingDelta(double currentAngle)
{
	currentAngle = abs(currentAngle);
	if (currentAngle == 0.0)
	{
		return speed * 180.0;
	}
	else
	{
		return speed * min(180.0, 8100.0 / currentAngle);
	}
}

void rotateFinishCallback()
{
	if (rotateMethod == ROTATE_NONE)
		return;

	switch (rotateMethod)
	{
	case ROTATE_NONE:
		break;
	case ROTATE_FRONT:
		cube.F();
		break;
	case ROTATE_BACK:
		cube.B();
		break;
	case ROTATE_LEFT:
		cube.L();
		break;
	case ROTATE_RIGHT:
		cube.R();
		break;
	case ROTATE_UP:
		cube.U();
		break;
	case ROTATE_DOWN:
		cube.D();
		break;
	case ROTATE_WHOLEX:
		cube.RotateUp();
		break;
	case ROTATE_WHOLEY:
		cube.RotateLeft();
		break;
	case ROTATE_WHOLEZ:
		cube.RotateClockwise();
		break;
	case ROTATE_FRONTi:
		cube.Fi();
		break;
	case ROTATE_BACKi:
		cube.Bi();
		break;
	case ROTATE_LEFTi:
		cube.Li();
		break;
	case ROTATE_RIGHTi:
		cube.Ri();
		break;
	case ROTATE_UPi:
		cube.Ui();
		break;
	case ROTATE_DOWNi:
		cube.Di();
		break;
	case ROTATE_WHOLEXi:
		cube.RotateDown();
		break;
	case ROTATE_WHOLEYi:
		cube.RotateRight();
		break;
	case ROTATE_WHOLEZi:
		cube.RotateCounterClockwise();
		break;
	default:
		break;
	}

	rotateAngle = 0.0;
	rotateMethod = ROTATE_NONE;

	playNext();
}

void nextFrame()
{
	double currentTime = glfwGetTime();

	if (rotateMethod != ROTATE_NONE)
	{
		double delta = easingDelta(rotateAngle) * (currentTime - lastPlayTime);
		rotateAngle += delta;
		if (rotateAngle >= finishAngle)
		{
			rotateFinishCallback();
		}
	}

	lastPlayTime = currentTime;
}

void finishCurrentRotate()
{
	rotateAngle = finishAngle;
	rotateFinishCallback();
}

void startRotate(CubeRotateMethod method)
{
	if (rotateMethod != ROTATE_NONE)
		finishCurrentRotate();
	rotateMethod = method;
	lastPlayTime = glfwGetTime();
}

void play(vector<CubeRotateMethod> &steps)
{
	if (steps.size() <= 0) return;
	stepsToPlay = steps;
	playIndex = 0;
	playing = true;
	startRotate(stepsToPlay[0]);
}

void playNext()
{
	if (playing)
	{
		++playIndex;
		if (playIndex < (ptrdiff_t)stepsToPlay.size())
		{
			startRotate(stepsToPlay[playIndex]);
		}
		else
		{
			stopPlay();
		}
	}
}

void stopPlay()
{
	playing = false;
	playIndex = 0;

}
#endif