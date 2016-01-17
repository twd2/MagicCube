#include "stdafx.h"
#include "Cube.h"

map<char, CubeColor> CharColorMap = {
	{'-', COLOR_UNUSED},
	{'W', COLOR_WHITE },
	{'O', COLOR_ORANGE},
	{'B', COLOR_BLUE  },
	{'R', COLOR_RED   },
	{'G', COLOR_GREEN },
	{'Y', COLOR_YELLOW}
};

map<CubeColor, char> ColorCharMap = {
	{COLOR_UNUSED, '-'},
	{COLOR_WHITE , 'W'},
	{COLOR_ORANGE, 'O'},
	{COLOR_BLUE  , 'B'},
	{COLOR_RED   , 'R'},
	{COLOR_GREEN , 'G'},
	{COLOR_YELLOW, 'Y'}
};

Cube::Cube()
{
	//-BO--Y-B---Y-B-R-Y-BO----B-----B-R---BO-W--B--W--B-RW---O--Y-----Y---R-Y--O------------R----O-W-----W----RW-G-O--YG----YG--R-YG-O---G-----G--R--G-O-W-G---W-G--RW-
	subCubes[0][0][0] = 0x063002;
	subCubes[1][0][0] = 0x060002;
	subCubes[2][0][0] = 0x060402;
	subCubes[0][0][1] = 0x003002;
	subCubes[1][0][1] = 0x000002;
	subCubes[2][0][1] = 0x000402;
	subCubes[0][0][2] = 0x503002;
	subCubes[1][0][2] = 0x500002;
	subCubes[2][0][2] = 0x500402;

	subCubes[0][1][0] = 0x063000;
	subCubes[1][1][0] = 0x060000;
	subCubes[2][1][0] = 0x060400;
	subCubes[0][1][1] = 0x003000;
	subCubes[1][1][1] = 0x000000;
	subCubes[2][1][1] = 0x000400;
	subCubes[0][1][2] = 0x503000;
	subCubes[1][1][2] = 0x500000;
	subCubes[2][1][2] = 0x500400;

	subCubes[0][2][0] = 0x063010;
	subCubes[1][2][0] = 0x060010;
	subCubes[2][2][0] = 0x060410;
	subCubes[0][2][1] = 0x003010;
	subCubes[1][2][1] = 0x000010;
	subCubes[2][2][1] = 0x000410;
	subCubes[0][2][2] = 0x503010;
	subCubes[1][2][2] = 0x500010;
	subCubes[2][2][2] = 0x500410;
}

Cube::Cube(const Cube &b)
{
	*this = b;
}

const Cube& Cube::operator=(const Cube &b)
{
	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int z = 0; z < 3; ++z)
			{
				subCubes[x][y][z] = b.subCubes[x][y][z];
			}
		}
	}
	return *this;
}

Cube::~Cube()
{
}

bool Cube::Deserialize(string data)
{
	if (data.length() != FORMAT1_LENGTH)
	{
		throw CubeError("Length mismatch");
		return false;
	}

	for (int z = 0; z < 3; ++z)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int x = 0; x < 3; ++x)
			{
				subCubes[x][y][z] =
					MAKE_CUBE(CharColorMap[data[((z * 3 + y) * 3 + x) * 6 + 0]],
							  CharColorMap[data[((z * 3 + y) * 3 + x) * 6 + 1]],
							  CharColorMap[data[((z * 3 + y) * 3 + x) * 6 + 2]],
							  CharColorMap[data[((z * 3 + y) * 3 + x) * 6 + 3]],
							  CharColorMap[data[((z * 3 + y) * 3 + x) * 6 + 4]],
							  CharColorMap[data[((z * 3 + y) * 3 + x) * 6 + 5]]);
			}
		}
	}

	return true;
}

string Cube::Serialize()
{
	string data;
	for (int z = 0; z < 3; ++z)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int x = 0; x < 3; ++x)
			{
				cube_t subcube = subCubes[x][y][z];
				data += ColorCharMap[GET_FRONT(subcube)];
				data += ColorCharMap[GET_BACK(subcube)];
				data += ColorCharMap[GET_LEFT(subcube)];
				data += ColorCharMap[GET_RIGHT(subcube)];
				data += ColorCharMap[GET_UP(subcube)];
				data += ColorCharMap[GET_DOWN(subcube)];
			}
		}
	}
	return data;
}

void Cube::SaveState()
{
	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int z = 0; z < 3; ++z)
			{
				oldSubCubes[x][y][z] = subCubes[x][y][z];
			}
		}
	}
}

void Cube::DoMethod(CubeRotateMethod method)
{
	switch (method)
	{
	case ROTATE_NONE:
	case ROTATE_NONEi:
		break;
	case ROTATE_FRONT:
		F();
		break;
	case ROTATE_BACK:
		B();
		break;
	case ROTATE_LEFT:
		L();
		break;
	case ROTATE_RIGHT:
		R();
		break;
	case ROTATE_UP:
		U();
		break;
	case ROTATE_DOWN:
		D();
		break;
	case ROTATE_FRONTi:
		Fi();
		break;
	case ROTATE_BACKi:
		Bi();
		break;
	case ROTATE_LEFTi:
		Li();
		break;
	case ROTATE_RIGHTi:
		Ri();
		break;
	case ROTATE_UPi:
		Ui();
		break;
	case ROTATE_DOWNi:
		Di();
		break;
	case ROTATE_WHOLEX:
		RotateUp();
		break;
	case ROTATE_WHOLEY:
		RotateLeft();
		break;
	case ROTATE_WHOLEZ:
		RotateClockwise();
		break;
	case ROTATE_WHOLEXi:
		RotateDown();
		break;
	case ROTATE_WHOLEYi:
		RotateRight();
		break;
	case ROTATE_WHOLEZi:
		RotateCounterClockwise();
		break;
	default:
		break;
	}
}

void Cube::R()
{
	SaveState();
	subCubes[2][0][0] = ROTATE_UP(oldSubCubes[2][2][0]);
	subCubes[2][0][1] = ROTATE_UP(oldSubCubes[2][1][0]);
	subCubes[2][0][2] = ROTATE_UP(oldSubCubes[2][0][0]);
	subCubes[2][1][0] = ROTATE_UP(oldSubCubes[2][2][1]);
	subCubes[2][1][1] = ROTATE_UP(oldSubCubes[2][1][1]);
	subCubes[2][1][2] = ROTATE_UP(oldSubCubes[2][0][1]);
	subCubes[2][2][0] = ROTATE_UP(oldSubCubes[2][2][2]);
	subCubes[2][2][1] = ROTATE_UP(oldSubCubes[2][1][2]);
	subCubes[2][2][2] = ROTATE_UP(oldSubCubes[2][0][2]);
}

void Cube::Ri()
{
	SaveState();
	subCubes[2][0][0] = ROTATE_DOWN(oldSubCubes[2][0][2]);
	subCubes[2][0][1] = ROTATE_DOWN(oldSubCubes[2][1][2]);
	subCubes[2][0][2] = ROTATE_DOWN(oldSubCubes[2][2][2]);
	subCubes[2][1][0] = ROTATE_DOWN(oldSubCubes[2][0][1]);
	subCubes[2][1][1] = ROTATE_DOWN(oldSubCubes[2][1][1]);
	subCubes[2][1][2] = ROTATE_DOWN(oldSubCubes[2][2][1]);
	subCubes[2][2][0] = ROTATE_DOWN(oldSubCubes[2][0][0]);
	subCubes[2][2][1] = ROTATE_DOWN(oldSubCubes[2][1][0]);
	subCubes[2][2][2] = ROTATE_DOWN(oldSubCubes[2][2][0]);
}

void Cube::L()
{
	SaveState();
	subCubes[0][0][0] = ROTATE_DOWN(oldSubCubes[0][0][2]);
	subCubes[0][0][1] = ROTATE_DOWN(oldSubCubes[0][1][2]);
	subCubes[0][0][2] = ROTATE_DOWN(oldSubCubes[0][2][2]);
	subCubes[0][1][0] = ROTATE_DOWN(oldSubCubes[0][0][1]);
	subCubes[0][1][1] = ROTATE_DOWN(oldSubCubes[0][1][1]);
	subCubes[0][1][2] = ROTATE_DOWN(oldSubCubes[0][2][1]);
	subCubes[0][2][0] = ROTATE_DOWN(oldSubCubes[0][0][0]);
	subCubes[0][2][1] = ROTATE_DOWN(oldSubCubes[0][1][0]);
	subCubes[0][2][2] = ROTATE_DOWN(oldSubCubes[0][2][0]);
}

void Cube::Li()
{
	SaveState();
	subCubes[0][0][0] = ROTATE_UP(oldSubCubes[0][2][0]);
	subCubes[0][0][1] = ROTATE_UP(oldSubCubes[0][1][0]);
	subCubes[0][0][2] = ROTATE_UP(oldSubCubes[0][0][0]);
	subCubes[0][1][0] = ROTATE_UP(oldSubCubes[0][2][1]);
	subCubes[0][1][1] = ROTATE_UP(oldSubCubes[0][1][1]);
	subCubes[0][1][2] = ROTATE_UP(oldSubCubes[0][0][1]);
	subCubes[0][2][0] = ROTATE_UP(oldSubCubes[0][2][2]);
	subCubes[0][2][1] = ROTATE_UP(oldSubCubes[0][1][2]);
	subCubes[0][2][2] = ROTATE_UP(oldSubCubes[0][0][2]);
}

void Cube::B()
{
	SaveState();
	subCubes[0][0][0] = ROTATE_CCLK(oldSubCubes[0][2][0]);
	subCubes[1][0][0] = ROTATE_CCLK(oldSubCubes[0][1][0]);
	subCubes[2][0][0] = ROTATE_CCLK(oldSubCubes[0][0][0]);
	subCubes[0][1][0] = ROTATE_CCLK(oldSubCubes[1][2][0]);
	subCubes[1][1][0] = ROTATE_CCLK(oldSubCubes[1][1][0]);
	subCubes[2][1][0] = ROTATE_CCLK(oldSubCubes[1][0][0]);
	subCubes[0][2][0] = ROTATE_CCLK(oldSubCubes[2][2][0]);
	subCubes[1][2][0] = ROTATE_CCLK(oldSubCubes[2][1][0]);
	subCubes[2][2][0] = ROTATE_CCLK(oldSubCubes[2][0][0]);
}

void Cube::Bi()
{
	SaveState();
	subCubes[0][0][0] = ROTATE_CLK(oldSubCubes[2][0][0]);
	subCubes[1][0][0] = ROTATE_CLK(oldSubCubes[2][1][0]);
	subCubes[2][0][0] = ROTATE_CLK(oldSubCubes[2][2][0]);
	subCubes[0][1][0] = ROTATE_CLK(oldSubCubes[1][0][0]);
	subCubes[1][1][0] = ROTATE_CLK(oldSubCubes[1][1][0]);
	subCubes[2][1][0] = ROTATE_CLK(oldSubCubes[1][2][0]);
	subCubes[0][2][0] = ROTATE_CLK(oldSubCubes[0][0][0]);
	subCubes[1][2][0] = ROTATE_CLK(oldSubCubes[0][1][0]);
	subCubes[2][2][0] = ROTATE_CLK(oldSubCubes[0][2][0]);
}

void Cube::D()
{
	SaveState();
	subCubes[0][0][0] = ROTATE_RIGHT(oldSubCubes[2][0][0]);
	subCubes[1][0][0] = ROTATE_RIGHT(oldSubCubes[2][0][1]);
	subCubes[2][0][0] = ROTATE_RIGHT(oldSubCubes[2][0][2]);
	subCubes[0][0][1] = ROTATE_RIGHT(oldSubCubes[1][0][0]);
	subCubes[1][0][1] = ROTATE_RIGHT(oldSubCubes[1][0][1]);
	subCubes[2][0][1] = ROTATE_RIGHT(oldSubCubes[1][0][2]);
	subCubes[0][0][2] = ROTATE_RIGHT(oldSubCubes[0][0][0]);
	subCubes[1][0][2] = ROTATE_RIGHT(oldSubCubes[0][0][1]);
	subCubes[2][0][2] = ROTATE_RIGHT(oldSubCubes[0][0][2]);
}

void Cube::Di()
{
	SaveState();
	subCubes[0][0][0] = ROTATE_LEFT(oldSubCubes[0][0][2]);
	subCubes[1][0][0] = ROTATE_LEFT(oldSubCubes[0][0][1]);
	subCubes[2][0][0] = ROTATE_LEFT(oldSubCubes[0][0][0]);
	subCubes[0][0][1] = ROTATE_LEFT(oldSubCubes[1][0][2]);
	subCubes[1][0][1] = ROTATE_LEFT(oldSubCubes[1][0][1]);
	subCubes[2][0][1] = ROTATE_LEFT(oldSubCubes[1][0][0]);
	subCubes[0][0][2] = ROTATE_LEFT(oldSubCubes[2][0][2]);
	subCubes[1][0][2] = ROTATE_LEFT(oldSubCubes[2][0][1]);
	subCubes[2][0][2] = ROTATE_LEFT(oldSubCubes[2][0][0]);
}

void Cube::F()
{
	SaveState();
	subCubes[0][0][2] = ROTATE_CLK(oldSubCubes[2][0][2]);
	subCubes[1][0][2] = ROTATE_CLK(oldSubCubes[2][1][2]);
	subCubes[2][0][2] = ROTATE_CLK(oldSubCubes[2][2][2]);
	subCubes[0][1][2] = ROTATE_CLK(oldSubCubes[1][0][2]);
	subCubes[1][1][2] = ROTATE_CLK(oldSubCubes[1][1][2]);
	subCubes[2][1][2] = ROTATE_CLK(oldSubCubes[1][2][2]);
	subCubes[0][2][2] = ROTATE_CLK(oldSubCubes[0][0][2]);
	subCubes[1][2][2] = ROTATE_CLK(oldSubCubes[0][1][2]);
	subCubes[2][2][2] = ROTATE_CLK(oldSubCubes[0][2][2]);
}

void Cube::Fi()
{
	SaveState();
	subCubes[0][0][2] = ROTATE_CCLK(oldSubCubes[0][2][2]);
	subCubes[1][0][2] = ROTATE_CCLK(oldSubCubes[0][1][2]);
	subCubes[2][0][2] = ROTATE_CCLK(oldSubCubes[0][0][2]);
	subCubes[0][1][2] = ROTATE_CCLK(oldSubCubes[1][2][2]);
	subCubes[1][1][2] = ROTATE_CCLK(oldSubCubes[1][1][2]);
	subCubes[2][1][2] = ROTATE_CCLK(oldSubCubes[1][0][2]);
	subCubes[0][2][2] = ROTATE_CCLK(oldSubCubes[2][2][2]);
	subCubes[1][2][2] = ROTATE_CCLK(oldSubCubes[2][1][2]);
	subCubes[2][2][2] = ROTATE_CCLK(oldSubCubes[2][0][2]);
}

void Cube::U()
{
	SaveState();
	subCubes[0][2][0] = ROTATE_LEFT(oldSubCubes[0][2][2]);
	subCubes[1][2][0] = ROTATE_LEFT(oldSubCubes[0][2][1]);
	subCubes[2][2][0] = ROTATE_LEFT(oldSubCubes[0][2][0]);
	subCubes[0][2][1] = ROTATE_LEFT(oldSubCubes[1][2][2]);
	subCubes[1][2][1] = ROTATE_LEFT(oldSubCubes[1][2][1]);
	subCubes[2][2][1] = ROTATE_LEFT(oldSubCubes[1][2][0]);
	subCubes[0][2][2] = ROTATE_LEFT(oldSubCubes[2][2][2]);
	subCubes[1][2][2] = ROTATE_LEFT(oldSubCubes[2][2][1]);
	subCubes[2][2][2] = ROTATE_LEFT(oldSubCubes[2][2][0]);
}

void Cube::Ui()
{
	SaveState();
	subCubes[0][2][0] = ROTATE_RIGHT(oldSubCubes[2][2][0]);
	subCubes[1][2][0] = ROTATE_RIGHT(oldSubCubes[2][2][1]);
	subCubes[2][2][0] = ROTATE_RIGHT(oldSubCubes[2][2][2]);
	subCubes[0][2][1] = ROTATE_RIGHT(oldSubCubes[1][2][0]);
	subCubes[1][2][1] = ROTATE_RIGHT(oldSubCubes[1][2][1]);
	subCubes[2][2][1] = ROTATE_RIGHT(oldSubCubes[1][2][2]);
	subCubes[0][2][2] = ROTATE_RIGHT(oldSubCubes[0][2][0]);
	subCubes[1][2][2] = ROTATE_RIGHT(oldSubCubes[0][2][1]);
	subCubes[2][2][2] = ROTATE_RIGHT(oldSubCubes[0][2][2]);
}

void Cube::RotateLeft()
{
	U();
	SaveState();
	subCubes[0][1][0] = ROTATE_LEFT(oldSubCubes[0][1][2]);
	subCubes[1][1][0] = ROTATE_LEFT(oldSubCubes[0][1][1]);
	subCubes[2][1][0] = ROTATE_LEFT(oldSubCubes[0][1][0]);
	subCubes[0][1][1] = ROTATE_LEFT(oldSubCubes[1][1][2]);
	subCubes[1][1][1] = ROTATE_LEFT(oldSubCubes[1][1][1]);
	subCubes[2][1][1] = ROTATE_LEFT(oldSubCubes[1][1][0]);
	subCubes[0][1][2] = ROTATE_LEFT(oldSubCubes[2][1][2]);
	subCubes[1][1][2] = ROTATE_LEFT(oldSubCubes[2][1][1]);
	subCubes[2][1][2] = ROTATE_LEFT(oldSubCubes[2][1][0]);
	Di();
}

void Cube::RotateRight()
{
	Ui();
	SaveState();
	subCubes[0][1][0] = ROTATE_RIGHT(oldSubCubes[2][1][0]);
	subCubes[1][1][0] = ROTATE_RIGHT(oldSubCubes[2][1][1]);
	subCubes[2][1][0] = ROTATE_RIGHT(oldSubCubes[2][1][2]);
	subCubes[0][1][1] = ROTATE_RIGHT(oldSubCubes[1][1][0]);
	subCubes[1][1][1] = ROTATE_RIGHT(oldSubCubes[1][1][1]);
	subCubes[2][1][1] = ROTATE_RIGHT(oldSubCubes[1][1][2]);
	subCubes[0][1][2] = ROTATE_RIGHT(oldSubCubes[0][1][0]);
	subCubes[1][1][2] = ROTATE_RIGHT(oldSubCubes[0][1][1]);
	subCubes[2][1][2] = ROTATE_RIGHT(oldSubCubes[0][1][2]);
	D();
}

void Cube::RotateUp()
{
	Li();
	SaveState();
	subCubes[1][0][0] = ROTATE_UP(oldSubCubes[1][2][0]);
	subCubes[1][0][1] = ROTATE_UP(oldSubCubes[1][1][0]);
	subCubes[1][0][2] = ROTATE_UP(oldSubCubes[1][0][0]);
	subCubes[1][1][0] = ROTATE_UP(oldSubCubes[1][2][1]);
	subCubes[1][1][1] = ROTATE_UP(oldSubCubes[1][1][1]);
	subCubes[1][1][2] = ROTATE_UP(oldSubCubes[1][0][1]);
	subCubes[1][2][0] = ROTATE_UP(oldSubCubes[1][2][2]);
	subCubes[1][2][1] = ROTATE_UP(oldSubCubes[1][1][2]);
	subCubes[1][2][2] = ROTATE_UP(oldSubCubes[1][0][2]);
	R();
}

void Cube::RotateDown()
{
	L();
	SaveState();
	subCubes[1][0][0] = ROTATE_DOWN(oldSubCubes[1][0][2]);
	subCubes[1][0][1] = ROTATE_DOWN(oldSubCubes[1][1][2]);
	subCubes[1][0][2] = ROTATE_DOWN(oldSubCubes[1][2][2]);
	subCubes[1][1][0] = ROTATE_DOWN(oldSubCubes[1][0][1]);
	subCubes[1][1][1] = ROTATE_DOWN(oldSubCubes[1][1][1]);
	subCubes[1][1][2] = ROTATE_DOWN(oldSubCubes[1][2][1]);
	subCubes[1][2][0] = ROTATE_DOWN(oldSubCubes[1][0][0]);
	subCubes[1][2][1] = ROTATE_DOWN(oldSubCubes[1][1][0]);
	subCubes[1][2][2] = ROTATE_DOWN(oldSubCubes[1][2][0]);
	Ri();
}

void Cube::RotateClockwise()
{
	F();
	SaveState();
	subCubes[0][0][1] = ROTATE_CLK(oldSubCubes[2][0][1]);
	subCubes[1][0][1] = ROTATE_CLK(oldSubCubes[2][1][1]);
	subCubes[2][0][1] = ROTATE_CLK(oldSubCubes[2][2][1]);
	subCubes[0][1][1] = ROTATE_CLK(oldSubCubes[1][0][1]);
	subCubes[1][1][1] = ROTATE_CLK(oldSubCubes[1][1][1]);
	subCubes[2][1][1] = ROTATE_CLK(oldSubCubes[1][2][1]);
	subCubes[0][2][1] = ROTATE_CLK(oldSubCubes[0][0][1]);
	subCubes[1][2][1] = ROTATE_CLK(oldSubCubes[0][1][1]);
	subCubes[2][2][1] = ROTATE_CLK(oldSubCubes[0][2][1]);
	Bi();
}

void Cube::RotateCounterClockwise()
{
	Fi();
	SaveState();
	subCubes[0][0][1] = ROTATE_CCLK(oldSubCubes[0][2][1]);
	subCubes[1][0][1] = ROTATE_CCLK(oldSubCubes[0][1][1]);
	subCubes[2][0][1] = ROTATE_CCLK(oldSubCubes[0][0][1]);
	subCubes[0][1][1] = ROTATE_CCLK(oldSubCubes[1][2][1]);
	subCubes[1][1][1] = ROTATE_CCLK(oldSubCubes[1][1][1]);
	subCubes[2][1][1] = ROTATE_CCLK(oldSubCubes[1][0][1]);
	subCubes[0][2][1] = ROTATE_CCLK(oldSubCubes[2][2][1]);
	subCubes[1][2][1] = ROTATE_CCLK(oldSubCubes[2][1][1]);
	subCubes[2][2][1] = ROTATE_CCLK(oldSubCubes[2][0][1]);
	B();
}

bool Cube::CheckL()
{
	CubeColor centre = GET_LEFT(subCubes[0][1][1]);
	return GET_LEFT(subCubes[0][0][0]) == centre &&
		   GET_LEFT(subCubes[0][0][1]) == centre &&
		   GET_LEFT(subCubes[0][0][2]) == centre &&
		   GET_LEFT(subCubes[0][1][0]) == centre &&
		   GET_LEFT(subCubes[0][1][1]) == centre &&
		   GET_LEFT(subCubes[0][1][2]) == centre &&
		   GET_LEFT(subCubes[0][2][0]) == centre &&
		   GET_LEFT(subCubes[0][2][1]) == centre &&
		   GET_LEFT(subCubes[0][2][2]) == centre;
}

bool Cube::CheckR()
{
	CubeColor centre = GET_RIGHT(subCubes[2][1][1]);
	return GET_RIGHT(subCubes[2][0][0]) == centre &&
		   GET_RIGHT(subCubes[2][0][1]) == centre &&
		   GET_RIGHT(subCubes[2][0][2]) == centre &&
		   GET_RIGHT(subCubes[2][1][0]) == centre &&
		   GET_RIGHT(subCubes[2][1][1]) == centre &&
		   GET_RIGHT(subCubes[2][1][2]) == centre &&
		   GET_RIGHT(subCubes[2][2][0]) == centre &&
		   GET_RIGHT(subCubes[2][2][1]) == centre &&
		   GET_RIGHT(subCubes[2][2][2]) == centre;
}

bool  Cube::CheckU()
{
	CubeColor centre = GET_UP(subCubes[1][2][1]);
	return GET_UP(subCubes[0][2][0]) == centre &&
		   GET_UP(subCubes[1][2][0]) == centre &&
		   GET_UP(subCubes[2][2][0]) == centre &&
		   GET_UP(subCubes[0][2][1]) == centre &&
		   GET_UP(subCubes[1][2][1]) == centre &&
		   GET_UP(subCubes[2][2][1]) == centre &&
		   GET_UP(subCubes[0][2][2]) == centre &&
		   GET_UP(subCubes[1][2][2]) == centre &&
		   GET_UP(subCubes[2][2][2]) == centre;
}

bool  Cube::CheckD()
{
	CubeColor centre = GET_DOWN(subCubes[1][0][1]);
	return GET_DOWN(subCubes[0][0][0]) == centre &&
		   GET_DOWN(subCubes[1][0][0]) == centre &&
		   GET_DOWN(subCubes[2][0][0]) == centre &&
		   GET_DOWN(subCubes[0][0][1]) == centre &&
		   GET_DOWN(subCubes[1][0][1]) == centre &&
		   GET_DOWN(subCubes[2][0][1]) == centre &&
		   GET_DOWN(subCubes[0][0][2]) == centre &&
		   GET_DOWN(subCubes[1][0][2]) == centre &&
		   GET_DOWN(subCubes[2][0][2]) == centre;
}

bool  Cube::CheckF()
{
	CubeColor centre = GET_FRONT(subCubes[1][1][2]);
	return GET_FRONT(subCubes[0][0][2]) == centre &&
		   GET_FRONT(subCubes[1][0][2]) == centre &&
		   GET_FRONT(subCubes[2][0][2]) == centre &&
		   GET_FRONT(subCubes[0][1][2]) == centre &&
		   GET_FRONT(subCubes[1][1][2]) == centre &&
		   GET_FRONT(subCubes[2][1][2]) == centre &&
		   GET_FRONT(subCubes[0][2][2]) == centre &&
		   GET_FRONT(subCubes[1][2][2]) == centre &&
		   GET_FRONT(subCubes[2][2][2]) == centre;
}

bool Cube::CheckB()
{
	CubeColor centre = GET_BACK(subCubes[1][1][0]);
	return GET_BACK(subCubes[0][0][0]) == centre &&
		   GET_BACK(subCubes[1][0][0]) == centre &&
		   GET_BACK(subCubes[2][0][0]) == centre &&
		   GET_BACK(subCubes[0][1][0]) == centre &&
		   GET_BACK(subCubes[1][1][0]) == centre &&
		   GET_BACK(subCubes[2][1][0]) == centre &&
		   GET_BACK(subCubes[0][2][0]) == centre &&
		   GET_BACK(subCubes[1][2][0]) == centre &&
		   GET_BACK(subCubes[2][2][0]) == centre;
}

bool Cube::Check()
{
	return CheckU() && CheckD() && CheckL() && CheckR() && CheckF() && CheckB();
}

bool Cube::operator==(const Cube &b)
{
	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int z = 0; z < 3; ++z)
			{
				if (subCubes[x][y][z] != b.subCubes[x][y][z])
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool Cube::operator!=(const Cube &b)
{
	return !(*this == b);
}