#include "stdafx.h"
#include "AlgorithmSolver.h"

AlgorithmSolver::~AlgorithmSolver()
{
}

void AlgorithmSolver::Solve()
{
	Stage1();
	Stage2();
	Stage3();
	Stage4();
	Stage5();
	Stage6();
	Stage7();
}

void AlgorithmSolver::GetUp(CubeColor color)
{
	if (GET_UP(cube.subCubes[U_CENTRE]) == color)
	{
		//nope
	}
	else if (GET_DOWN(cube.subCubes[D_CENTRE]) == color)
	{
		Do(WHOLEX);
		Do(WHOLEX);
		//can also do WHOLEZ * 2
	}
	else if (GET_LEFT(cube.subCubes[L_CENTRE]) == color)
	{
		Do(WHOLEZ);
	}
	else if (GET_RIGHT(cube.subCubes[R_CENTRE]) == color)
	{
		Do(WHOLEZi);
	}
	else if (GET_FRONT(cube.subCubes[F_CENTRE]) == color)
	{
		Do(WHOLEX);
	}
	else if (GET_BACK(cube.subCubes[B_CENTRE]) == color)
	{
		Do(WHOLEXi);
	}
}

void AlgorithmSolver::GetDown(CubeColor color)
{
	if (GET_UP(cube.subCubes[U_CENTRE]) == color)
	{
		Do(WHOLEX);
		Do(WHOLEX);
		//can also do WHOLEZ * 2
	}
	else if (GET_DOWN(cube.subCubes[D_CENTRE]) == color)
	{
		//nope
	}
	else if (GET_LEFT(cube.subCubes[L_CENTRE]) == color)
	{
		Do(WHOLEZi);
	}
	else if (GET_RIGHT(cube.subCubes[R_CENTRE]) == color)
	{
		Do(WHOLEZ);
	}
	else if (GET_FRONT(cube.subCubes[F_CENTRE]) == color)
	{
		Do(WHOLEXi);
	}
	else if (GET_BACK(cube.subCubes[B_CENTRE]) == color)
	{
		Do(WHOLEX);
	}
}

void AlgorithmSolver::GetLeft(CubeColor color)
{
	if (GET_UP(cube.subCubes[U_CENTRE]) == color)
	{
		Do(WHOLEZi);
	}
	else if (GET_DOWN(cube.subCubes[D_CENTRE]) == color)
	{
		Do(WHOLEZ);
	}
	else if (GET_LEFT(cube.subCubes[L_CENTRE]) == color)
	{
		//nope
	}
	else if (GET_RIGHT(cube.subCubes[R_CENTRE]) == color)
	{
		Do(WHOLEY);
		Do(WHOLEY);
		//can also WHOLEZ * 2
	}
	else if (GET_FRONT(cube.subCubes[F_CENTRE]) == color)
	{
		Do(WHOLEY);
	}
	else if (GET_BACK(cube.subCubes[B_CENTRE]) == color)
	{
		Do(WHOLEYi);
	}
}

void AlgorithmSolver::GetRight(CubeColor color)
{
	if (GET_UP(cube.subCubes[U_CENTRE]) == color)
	{
		Do(WHOLEZ);
	}
	else if (GET_DOWN(cube.subCubes[D_CENTRE]) == color)
	{
		Do(WHOLEZi);
	}
	else if (GET_LEFT(cube.subCubes[L_CENTRE]) == color)
	{
		Do(WHOLEY);
		Do(WHOLEY);
		//can also WHOLEZ * 2
	}
	else if (GET_RIGHT(cube.subCubes[R_CENTRE]) == color)
	{
		//nope
	}
	else if (GET_FRONT(cube.subCubes[F_CENTRE]) == color)
	{
		Do(WHOLEYi);
	}
	else if (GET_BACK(cube.subCubes[B_CENTRE]) == color)
	{
		Do(WHOLEY);
	}
}

void AlgorithmSolver::GetFront(CubeColor color)
{
	if (GET_UP(cube.subCubes[U_CENTRE]) == color)
	{
		Do(WHOLEXi);
	}
	else if (GET_DOWN(cube.subCubes[D_CENTRE]) == color)
	{
		Do(WHOLEX);
	}
	else if (GET_LEFT(cube.subCubes[L_CENTRE]) == color)
	{
		Do(WHOLEYi);
	}
	else if (GET_RIGHT(cube.subCubes[R_CENTRE]) == color)
	{
		Do(WHOLEY);
	}
	else if (GET_FRONT(cube.subCubes[F_CENTRE]) == color)
	{
		//nope
	}
	else if (GET_BACK(cube.subCubes[B_CENTRE]) == color)
	{
		Do(WHOLEX);
		Do(WHOLEX);
		//can also WHOLEY * 2
	}
}

void AlgorithmSolver::GetBack(CubeColor color)
{
	if (GET_UP(cube.subCubes[U_CENTRE]) == color)
	{
		Do(WHOLEX);
	}
	else if (GET_DOWN(cube.subCubes[D_CENTRE]) == color)
	{
		Do(WHOLEXi);
	}
	else if (GET_LEFT(cube.subCubes[L_CENTRE]) == color)
	{
		Do(WHOLEY);
	}
	else if (GET_RIGHT(cube.subCubes[R_CENTRE]) == color)
	{
		Do(WHOLEYi);
	}
	else if (GET_FRONT(cube.subCubes[F_CENTRE]) == color)
	{
		Do(WHOLEX);
		Do(WHOLEX);
		//can also WHOLEY * 2
	}
	else if (GET_BACK(cube.subCubes[B_CENTRE]) == color)
	{
		//nope
	}
}

void AlgorithmSolver::Stage1()
{
	//white centre piece on the top face
	GetUp(COLOR_WHITE);

	//blue, orange, green, red
	GetRight(COLOR_BLUE);

	auto colors = { COLOR_BLUE, COLOR_ORANGE, COLOR_GREEN, COLOR_RED };
	bool isFirst = true;

	for each (CubeColor color in colors)
	{
		//find ????/white edge and put it DR
		FIND_EDGE(cube.subCubes, color, COLOR_WHITE,
		{
			//FL
			Do(FRONTi); // -> FD
			Do(DOWN); // -> DR
			if (!isFirst) //is not first, need to restore
			{
				Do(FRONT);
			}
		},
		{
			//FR
			Do(RIGHTi); // -> DR
		},
		{
			//FU
			Do(FRONT); // -> FR
			Do(RIGHTi); // -> DR
			if (!isFirst) //is not first, need to restore
			{
				Do(FRONTi);
			}
		},
		{
			//FD
			Do(DOWN); // -> DR
		},
		{
			//BL
			Do(BACK); // -> BD
			Do(DOWNi); // -> DR
			if (!isFirst) //is not first, need to restore
			{
				Do(BACKi);
			}
		},
		{
			//BR
			Do(RIGHT); // -> DR
		},
		{
			//BU
			Do(BACKi); // -> BR
			Do(RIGHT); // -> DR
			if (!isFirst) //is not first, need to restore
			{
				Do(BACK);
			}
		},
		{
			//BD
			Do(DOWNi); // -> DR
		},
		{
			//UL
			Do(LEFT); // -> FL
			Do(FRONTi); // -> FD
			Do(DOWN); // -> DR
			if (!isFirst) //is not first, need to restore
			{
				Do(FRONT);
				Do(LEFTi);
			}
		},
		{
			//UR
			Do(RIGHT); // -> BR
			Do(RIGHT); // -> DR
		},
		{
			//DL
			Do(DOWN); // -> BR
			Do(DOWN); // -> DR
		},
		{
			//DR
			//nope
		});

		// -> UR
		Do(RIGHT); // -> FR
		Do(RIGHT); // -> UR

		if (GET_RIGHT(cube.subCubes[UR_EDGE]) == COLOR_WHITE)
		{
			//state2
			Do(RIGHTi);
			Do(UP);
			Do(FRONTi);
			Do(UPi);
		}
		if (color != *(colors.end() - 1))
		{
			Do(WHOLEY);
		}
		isFirst = false;
	}
	/*if (IsColorMatch2(cube.subCubes[FD_EDGE], COLOR_WHITE, COLOR_BLUE))
	{
		Do(DOWN);
		Do(DOWN);
		Do(BACK);
		Do(BACK);
	}
	else
	{
		printf("orz\n");
	}*/

}

void AlgorithmSolver::Stage2()
{

}

void AlgorithmSolver::Stage3()
{

}

void AlgorithmSolver::Stage4()
{

}

void AlgorithmSolver::Stage5()
{

}

void AlgorithmSolver::Stage6()
{

}

void AlgorithmSolver::Stage7()
{

}