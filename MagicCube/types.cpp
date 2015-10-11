#include "stdafx.h"
#include "types.h"

map<CubeRotateMethod, string> CubeRotateMethodName = 
{
	{ROTATE_NONE, "NONE"},
	{ROTATE_FRONT, "F" },
	{ROTATE_BACK, "B" },
	{ROTATE_LEFT, "L"},
	{ROTATE_RIGHT, "R"},
	{ROTATE_UP, "U"},
	{ROTATE_DOWN, "D"},
	{ROTATE_WHOLEX, "X"},
	{ROTATE_WHOLEY, "Y"},
	{ROTATE_WHOLEZ, "Z"},
	{ROTATE_NONEi, "NONE"},
	{ROTATE_FRONTi, "Fi"},
	{ROTATE_BACKi, "Bi"},
	{ROTATE_LEFTi, "Li"},
	{ROTATE_RIGHTi, "Ri"},
	{ROTATE_UPi, "Ui"},
	{ROTATE_DOWNi, "Di"},
	{ROTATE_WHOLEXi, "Xi"},
	{ROTATE_WHOLEYi, "Yi"},
	{ROTATE_WHOLEZi, "Zi"}
};