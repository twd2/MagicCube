#include "stdafx.h"
#include "Cube.h"


Cube::Cube()
{
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


Cube::~Cube()
{
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

void Cube::RotateClk()
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

void Cube::RotateCClk()
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
	F();
}