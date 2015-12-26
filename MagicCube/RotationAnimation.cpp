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

	cube.DoMethod(rotateMethod);

	rotateAngle = 0.0;
	rotateMethod = ROTATE_NONE;

	if (playing)
	{
		playNext();
	}
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

void play(CubeSteps &steps)
{
	if (steps.size() <= 0) return;
	stepsToPlay = steps;
	playIndex = 0;
	playing = true;
	startRotate(stepsToPlay[0]);
}

void playNext()
{
	if (!playing)
	{
		return;
	}

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

void stopPlay()
{
	playing = false;
	playIndex = 0;
}
#endif