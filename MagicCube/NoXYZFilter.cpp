#include "stdafx.h"
#include "NoXYZFilter.h"

map<CubeRotateMethod, map<CubeRotateMethod, CubeRotateMethod> > XYZMapTables =
{
	{ ROTATE_NONE, {
		{ ROTATE_NONE, ROTATE_NONE },
		{ ROTATE_FRONT, ROTATE_FRONT },
		{ ROTATE_BACK, ROTATE_BACK },
		{ ROTATE_LEFT, ROTATE_LEFT },
		{ ROTATE_RIGHT, ROTATE_RIGHT },
		{ ROTATE_UP, ROTATE_UP },
		{ ROTATE_DOWN, ROTATE_DOWN },
		{ ROTATE_WHOLEX, ROTATE_WHOLEX },
		{ ROTATE_WHOLEY, ROTATE_WHOLEY },
		{ ROTATE_WHOLEZ, ROTATE_WHOLEZ },
		{ ROTATE_NONEi, ROTATE_NONE },
		{ ROTATE_FRONTi, ROTATE_FRONTi },
		{ ROTATE_BACKi, ROTATE_BACKi },
		{ ROTATE_LEFTi, ROTATE_LEFTi },
		{ ROTATE_RIGHTi, ROTATE_RIGHTi },
		{ ROTATE_UPi, ROTATE_UPi },
		{ ROTATE_DOWNi, ROTATE_DOWNi },
		{ ROTATE_WHOLEXi, ROTATE_WHOLEXi },
		{ ROTATE_WHOLEYi, ROTATE_WHOLEYi },
		{ ROTATE_WHOLEZi, ROTATE_WHOLEZi }
	} },
	{ ROTATE_WHOLEX, {
		{ ROTATE_NONE, ROTATE_NONE },
		{ ROTATE_FRONT, ROTATE_DOWN },
		{ ROTATE_BACK, ROTATE_UP },
		{ ROTATE_LEFT, ROTATE_LEFT },
		{ ROTATE_RIGHT, ROTATE_RIGHT },
		{ ROTATE_UP, ROTATE_FRONT },
		{ ROTATE_DOWN, ROTATE_BACK },
		{ ROTATE_WHOLEX, ROTATE_WHOLEX },
		{ ROTATE_WHOLEY, ROTATE_WHOLEZ },
		{ ROTATE_WHOLEZ, ROTATE_WHOLEYi },
		{ ROTATE_NONEi, ROTATE_NONE },
		{ ROTATE_FRONTi, ROTATE_DOWNi },
		{ ROTATE_BACKi, ROTATE_UPi },
		{ ROTATE_LEFTi, ROTATE_LEFTi },
		{ ROTATE_RIGHTi, ROTATE_RIGHTi },
		{ ROTATE_UPi, ROTATE_FRONTi },
		{ ROTATE_DOWNi, ROTATE_BACKi },
		{ ROTATE_WHOLEXi, ROTATE_WHOLEXi },
		{ ROTATE_WHOLEYi, ROTATE_WHOLEZi },
		{ ROTATE_WHOLEZi, ROTATE_WHOLEY }
	} },
	{ ROTATE_WHOLEY, {
		{ ROTATE_NONE, ROTATE_NONE },
		{ ROTATE_FRONT, ROTATE_RIGHT },
		{ ROTATE_BACK, ROTATE_LEFT },
		{ ROTATE_LEFT, ROTATE_FRONT },
		{ ROTATE_RIGHT, ROTATE_BACK },
		{ ROTATE_UP, ROTATE_UP },
		{ ROTATE_DOWN, ROTATE_DOWN },
		{ ROTATE_WHOLEX, ROTATE_WHOLEZi },
		{ ROTATE_WHOLEY, ROTATE_WHOLEY },
		{ ROTATE_WHOLEZ, ROTATE_WHOLEX },
		{ ROTATE_NONEi, ROTATE_NONE },
		{ ROTATE_FRONTi, ROTATE_RIGHTi },
		{ ROTATE_BACKi, ROTATE_LEFTi },
		{ ROTATE_LEFTi, ROTATE_FRONTi },
		{ ROTATE_RIGHTi, ROTATE_BACKi },
		{ ROTATE_UPi, ROTATE_UPi },
		{ ROTATE_DOWNi, ROTATE_DOWNi },
		{ ROTATE_WHOLEXi, ROTATE_WHOLEZ },
		{ ROTATE_WHOLEYi, ROTATE_WHOLEYi },
		{ ROTATE_WHOLEZi, ROTATE_WHOLEXi }
	} },
	{ ROTATE_WHOLEZ, {
		{ ROTATE_NONE, ROTATE_NONE },
		{ ROTATE_FRONT, ROTATE_FRONT },
		{ ROTATE_BACK, ROTATE_BACK },
		{ ROTATE_LEFT, ROTATE_DOWN },
		{ ROTATE_RIGHT, ROTATE_UP },
		{ ROTATE_UP, ROTATE_LEFT },
		{ ROTATE_DOWN, ROTATE_RIGHT },
		{ ROTATE_WHOLEX, ROTATE_WHOLEY },
		{ ROTATE_WHOLEY, ROTATE_WHOLEXi },
		{ ROTATE_WHOLEZ, ROTATE_WHOLEZ },
		{ ROTATE_NONEi, ROTATE_NONE },
		{ ROTATE_FRONTi, ROTATE_FRONTi },
		{ ROTATE_BACKi, ROTATE_BACKi },
		{ ROTATE_LEFTi, ROTATE_DOWNi },
		{ ROTATE_RIGHTi, ROTATE_UPi },
		{ ROTATE_UPi, ROTATE_LEFTi },
		{ ROTATE_DOWNi, ROTATE_RIGHTi },
		{ ROTATE_WHOLEXi, ROTATE_WHOLEYi },
		{ ROTATE_WHOLEYi, ROTATE_WHOLEX },
		{ ROTATE_WHOLEZi, ROTATE_WHOLEZi }
	} },
	{ ROTATE_WHOLEXi, {
		{ ROTATE_NONE, ROTATE_NONE },
		{ ROTATE_FRONT, ROTATE_UP },
		{ ROTATE_BACK, ROTATE_DOWN },
		{ ROTATE_LEFT, ROTATE_LEFT },
		{ ROTATE_RIGHT, ROTATE_RIGHT },
		{ ROTATE_UP, ROTATE_BACK },
		{ ROTATE_DOWN, ROTATE_FRONT },
		{ ROTATE_WHOLEX, ROTATE_WHOLEX },
		{ ROTATE_WHOLEY, ROTATE_WHOLEZi },
		{ ROTATE_WHOLEZ, ROTATE_WHOLEY },
		{ ROTATE_NONEi, ROTATE_NONE },
		{ ROTATE_FRONTi, ROTATE_UPi }, 
		{ ROTATE_BACKi, ROTATE_DOWNi },
		{ ROTATE_LEFTi, ROTATE_LEFTi },
		{ ROTATE_RIGHTi, ROTATE_RIGHTi },
		{ ROTATE_UPi, ROTATE_BACKi },
		{ ROTATE_DOWNi, ROTATE_FRONTi },
		{ ROTATE_WHOLEXi, ROTATE_WHOLEXi },
		{ ROTATE_WHOLEYi, ROTATE_WHOLEZ },
		{ ROTATE_WHOLEZi, ROTATE_WHOLEYi }
	} },
	{ ROTATE_WHOLEYi, {
		{ ROTATE_NONE, ROTATE_NONE },
		{ ROTATE_FRONT, ROTATE_LEFT },
		{ ROTATE_BACK, ROTATE_RIGHT },
		{ ROTATE_LEFT, ROTATE_BACK },
		{ ROTATE_RIGHT, ROTATE_FRONT },
		{ ROTATE_UP, ROTATE_UP },
		{ ROTATE_DOWN, ROTATE_DOWN },
		{ ROTATE_WHOLEX, ROTATE_WHOLEZ },
		{ ROTATE_WHOLEY, ROTATE_WHOLEY },
		{ ROTATE_WHOLEZ, ROTATE_WHOLEXi },
		{ ROTATE_NONEi, ROTATE_NONE },
		{ ROTATE_FRONTi, ROTATE_LEFTi },
		{ ROTATE_BACKi, ROTATE_RIGHTi },
		{ ROTATE_LEFTi, ROTATE_BACKi },
		{ ROTATE_RIGHTi, ROTATE_FRONTi },
		{ ROTATE_UPi, ROTATE_UPi },
		{ ROTATE_DOWNi, ROTATE_DOWNi },
		{ ROTATE_WHOLEXi, ROTATE_WHOLEZi },
		{ ROTATE_WHOLEYi, ROTATE_WHOLEYi },
		{ ROTATE_WHOLEZi, ROTATE_WHOLEX }
	} },
	{ ROTATE_WHOLEZi, {
		{ ROTATE_NONE, ROTATE_NONE },
		{ ROTATE_FRONT, ROTATE_FRONT },
		{ ROTATE_BACK, ROTATE_BACK },
		{ ROTATE_LEFT, ROTATE_UP },
		{ ROTATE_RIGHT, ROTATE_DOWN },
		{ ROTATE_UP, ROTATE_RIGHT },
		{ ROTATE_DOWN, ROTATE_LEFT },
		{ ROTATE_WHOLEX, ROTATE_WHOLEYi },
		{ ROTATE_WHOLEY, ROTATE_WHOLEX },
		{ ROTATE_WHOLEZ, ROTATE_WHOLEZ },
		{ ROTATE_NONEi, ROTATE_NONE },
		{ ROTATE_FRONTi, ROTATE_FRONTi },
		{ ROTATE_BACKi, ROTATE_BACKi },
		{ ROTATE_LEFTi, ROTATE_UPi },
		{ ROTATE_RIGHTi, ROTATE_DOWNi },
		{ ROTATE_UPi, ROTATE_RIGHTi },
		{ ROTATE_DOWNi, ROTATE_LEFTi },
		{ ROTATE_WHOLEXi, ROTATE_WHOLEY },
		{ ROTATE_WHOLEYi, ROTATE_WHOLEXi },
		{ ROTATE_WHOLEZi, ROTATE_WHOLEZi }
	} }
};

// above tables were generated by following code:
/*for (CubeRotateMethod wholeMethod = ROTATE_NONE; wholeMethod <= ROTATE_WHOLEZi; wholeMethod = (CubeRotateMethod)((int)wholeMethod + 1))
{
	if (!(wholeMethod == ROTATE_NONE || isWholeRotate(wholeMethod)))
	{
		continue;
	}

	printf("{ %s, {\n", CubeRotateMethodEnumName[wholeMethod].c_str());
	for (CubeRotateMethod src = ROTATE_NONE; src <= ROTATE_WHOLEZi; src = (CubeRotateMethod)((int)src + 1))
	{
		for (CubeRotateMethod des = ROTATE_NONE; des <= ROTATE_WHOLEZi; des = (CubeRotateMethod)((int)des + 1))
		{
			Cube a, b;
			a.DoMethod(wholeMethod);
			a.DoMethod(src);
			a.DoMethod(inverse(wholeMethod));
			b.DoMethod(des);
			if (a == b)
			{
				// found
				printf("\t{%s, %s}", CubeRotateMethodEnumName[src].c_str(), CubeRotateMethodEnumName[des].c_str());

				break;
			}
		}
		printf(",\n");
	}
	printf("} },\n");
}*/

CubeSteps NoXYZFilter::Filter(CubeSteps &steps)
{
	CubeSteps newSteps = steps;
	auto currentMap = XYZMapTables[ROTATE_NONE];
	size_t size = newSteps.size();
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size; ++i)
	{
		auto step = currentMap[newSteps[i]];
		if (isWholeRotate(step))
		{
			CombineMapTable(currentMap, XYZMapTables[step]);
			newSteps[i] = ROTATE_NONE;
		}
		else
		{
			newSteps[i] = step;
		}
	}
	CubeSteps newNewSteps;
	copySteps(newSteps, newNewSteps);
	return newNewSteps;
}

NoXYZFilter::~NoXYZFilter()
{
}

void NoXYZFilter::CombineMapTable(map<CubeRotateMethod, CubeRotateMethod> &target, map<CubeRotateMethod, CubeRotateMethod> &mapTable)
{
	for (CubeRotateMethod m = ROTATE_NONE; m <= ROTATE_WHOLEZi; m = (CubeRotateMethod)((int)m + 1))
	{
		target[m] = mapTable[target[m]];
	}
}
