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

void AlgorithmSolver::MoveToUp(CubeColor color)
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

void AlgorithmSolver::MoveToDown(CubeColor color)
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

void AlgorithmSolver::MoveToLeft(CubeColor color)
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

void AlgorithmSolver::MoveToRight(CubeColor color)
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

void AlgorithmSolver::MoveToFront(CubeColor color)
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
		Do(WHOLEY);
		Do(WHOLEY);
		//can also WHOLEX * 2
	}
}

void AlgorithmSolver::MoveToBack(CubeColor color)
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
	MoveToUp(COLOR_WHITE);

	//blue, orange, green, red
	MoveToRight(COLOR_BLUE);

	auto colors = { COLOR_BLUE, COLOR_ORANGE, COLOR_GREEN, COLOR_RED };
	CubeColor lastColor = *(colors.end() - 1);
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

		//is not last, rotate to next color
		if (color != lastColor)
		{
			Do(WHOLEY);
		}
		isFirst = false;
	}
}

void AlgorithmSolver::Stage2()
{

	vector<vector<CubeColor> > colors = { { COLOR_GREEN, COLOR_RED }, { COLOR_RED, COLOR_BLUE }, { COLOR_BLUE, COLOR_ORANGE }, { COLOR_ORANGE, COLOR_GREEN } };
	auto lastColor = *(colors.end()-1);

	for each (auto color in colors)
	{
		//find white/????/???? corner and put it FRD
		FIND_CORNER(cube.subCubes, COLOR_WHITE, color[0], color[1],
		{
			//FLU
			Do(LEFT); // -> FLD
			Do(DOWN); // -> FRD
			Do(LEFTi); //restore left
		},
		{
			//FLD
			Do(DOWN); // -> FRD
		},
		{
			//FRU
			Do(RIGHTi); // -> FRD
			Do(DOWNi); // -> FLD to save
			Do(RIGHT); //restore right
			Do(DOWN); // -> FRD
		},
		{
			//FRD
			//nope
		},
		{
			//BLU
			Do(LEFTi); // -> BLD
			Do(DOWN); // -> FLD
			Do(DOWN); // -> FRD
			Do(LEFT); //restore left
		},
		{
			//BLD
			Do(DOWN); // -> FLD
			Do(DOWN); // -> FRD
		},
		{
			//BRU
			Do(BACKi); // -> BRD
			Do(DOWNi); // -> FRD
			Do(BACK); //restore back
		},
		{
			//BRD
			Do(DOWNi); // -> FRD
		});
		do
		{
			Do(RIGHTi);
			Do(DOWNi);
			Do(RIGHT);
			Do(DOWN);
		} while (!(GET_UP(cube.subCubes[FRU_CORNER]) == COLOR_WHITE &&
			GET_FRONT(cube.subCubes[FRU_CORNER]) == color[0] &&
			GET_RIGHT(cube.subCubes[FRU_CORNER]) == color[1]));

		if (color != lastColor)
		{
			Do(WHOLEY); //rotate to next color
		}
	}
}

bool AlgorithmSolver::CheckStage3()
{
	CubeColor leftCentre = GET_LEFT(cube.subCubes[0][1][1]);
	bool checkLeft = GET_LEFT(cube.subCubes[0][0][0]) == leftCentre &&
					 GET_LEFT(cube.subCubes[0][0][1]) == leftCentre &&
					 GET_LEFT(cube.subCubes[0][0][2]) == leftCentre &&
					 GET_LEFT(cube.subCubes[0][1][0]) == leftCentre &&
					 GET_LEFT(cube.subCubes[0][1][1]) == leftCentre &&
					 GET_LEFT(cube.subCubes[0][1][2]) == leftCentre;

	CubeColor rightCentre = GET_RIGHT(cube.subCubes[2][1][1]);
	bool checkRight = GET_RIGHT(cube.subCubes[2][0][0]) == rightCentre &&
					  GET_RIGHT(cube.subCubes[2][0][1]) == rightCentre &&
					  GET_RIGHT(cube.subCubes[2][0][2]) == rightCentre &&
					  GET_RIGHT(cube.subCubes[2][1][0]) == rightCentre &&
					  GET_RIGHT(cube.subCubes[2][1][1]) == rightCentre &&
					  GET_RIGHT(cube.subCubes[2][1][2]) == rightCentre;

	CubeColor frontCentre = GET_FRONT(cube.subCubes[1][1][2]);
	bool checkFront = GET_FRONT(cube.subCubes[0][0][2]) == frontCentre &&
					  GET_FRONT(cube.subCubes[1][0][2]) == frontCentre &&
					  GET_FRONT(cube.subCubes[2][0][2]) == frontCentre &&
					  GET_FRONT(cube.subCubes[0][1][2]) == frontCentre &&
					  GET_FRONT(cube.subCubes[1][1][2]) == frontCentre &&
					  GET_FRONT(cube.subCubes[2][1][2]) == frontCentre;

	CubeColor backCentre = GET_BACK(cube.subCubes[1][1][0]);
	bool checkBack = GET_BACK(cube.subCubes[0][0][0]) == backCentre &&
					 GET_BACK(cube.subCubes[1][0][0]) == backCentre &&
					 GET_BACK(cube.subCubes[2][0][0]) == backCentre &&
					 GET_BACK(cube.subCubes[0][1][0]) == backCentre &&
					 GET_BACK(cube.subCubes[1][1][0]) == backCentre &&
					 GET_BACK(cube.subCubes[2][1][0]) == backCentre;

	return cube.CheckD() && checkLeft && checkRight && checkFront && checkBack;
}

void AlgorithmSolver::Stage3()
{
	MoveToUp(COLOR_YELLOW);

	map<CubeColor, CubeColor> leftColor = {
			{ COLOR_RED, COLOR_BLUE },
			{ COLOR_BLUE, COLOR_ORANGE },
			{ COLOR_ORANGE, COLOR_GREEN },
			{ COLOR_GREEN, COLOR_RED }
	};

	map<CubeColor, CubeColor> rightColor = {
			{ COLOR_BLUE, COLOR_RED },
			{ COLOR_ORANGE, COLOR_BLUE },
			{ COLOR_GREEN, COLOR_ORANGE },
			{ COLOR_RED, COLOR_GREEN}
	};

	while (!CheckStage3())
	{
		vector<CubeColor> colors = { COLOR_RED, COLOR_GREEN, COLOR_ORANGE, COLOR_BLUE };

		CubeColor foundColor = COLOR_INVALID;
		for each (auto color in colors)
		{
			MoveToFront(color);
			if (GET_FRONT(cube.subCubes[FU_EDGE]) == color && GET_UP(cube.subCubes[FU_EDGE]) != COLOR_YELLOW)
			{
				//nope, found vertical row of color
				foundColor = color;
			}
			else if (GET_RIGHT(cube.subCubes[UR_EDGE]) == color && GET_UP(cube.subCubes[UR_EDGE]) != COLOR_YELLOW)
			{
				Do(UP);
				foundColor = color;
			}
			else if (GET_BACK(cube.subCubes[BU_EDGE]) == color && GET_UP(cube.subCubes[BU_EDGE]) != COLOR_YELLOW)
			{
				Do(UP);
				Do(UP);
				foundColor = color;
			}
			else if (GET_LEFT(cube.subCubes[UL_EDGE]) == color && GET_UP(cube.subCubes[UL_EDGE]) != COLOR_YELLOW)
			{
				Do(UPi);
				foundColor = color;
			}
			else
			{
				//sad :(
			}

			if (foundColor != COLOR_INVALID)
			{
				break;
			}
		}

		if (foundColor == COLOR_INVALID)
		{
			//truly SAD
			for each (auto color in colors)
			{
				MoveToFront(color);
				if ((GET_FRONT(cube.subCubes[FL_EDGE]) != color || GET_LEFT(cube.subCubes[FL_EDGE]) != leftColor[color]) &&
					GET_FRONT(cube.subCubes[FL_EDGE]) != COLOR_YELLOW && GET_LEFT(cube.subCubes[FL_EDGE]) != COLOR_YELLOW)
				{
					Do(FRONT);
					Do(UP);
					Do(FRONTi);
					Do(UPi);
					Do(LEFTi);
					Do(UPi);
					Do(LEFT);
					Do(UP);
					break;
				}
				if (GET_FRONT(cube.subCubes[FR_EDGE]) != color || GET_RIGHT(cube.subCubes[FR_EDGE]) != rightColor[color] &&
					GET_FRONT(cube.subCubes[FR_EDGE]) != COLOR_YELLOW && GET_RIGHT(cube.subCubes[FR_EDGE]) != COLOR_YELLOW)
				{
					Do(FRONTi);
					Do(UPi);
					Do(FRONT);
					Do(UP);
					Do(RIGHT);
					Do(UP);
					Do(RIGHTi);
					Do(UPi);
					break;
				}
			}
		}
		else
		{
			//move found edge to correct position
			if (GET_UP(cube.subCubes[FU_EDGE]) == leftColor[foundColor])
			{
				//2)
				Do(UPi);
				Do(LEFTi);
				Do(UP);
				Do(LEFT);
				Do(UP);
				Do(FRONT);
				Do(UPi);
				Do(FRONTi);
			}
			else if (GET_UP(cube.subCubes[FU_EDGE]) == rightColor[foundColor])
			{
				//1)
				Do(UP);
				Do(RIGHT);
				Do(UPi);
				Do(RIGHTi);
				Do(UPi);
				Do(FRONTi);
				Do(UP);
				Do(FRONT);
			}
			else
			{
				//assert false
				throw SolverError();
			}
		}
	}
	
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