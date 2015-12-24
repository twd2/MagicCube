#pragma once

#include <map>
#include "CubeError.h"

extern map<char, CubeColor> CharColorMap;
extern map<CubeColor, char> ColorCharMap;

class Cube
{
public:
	Cube();

	// copy
	Cube(const Cube&);

	// copy
	const Cube& operator=(const Cube&);


	~Cube();

	cube_t subCubes[3][3][3];
	//              x  y  z
	/*
	    y
	    |
	    |
	    |
	    |________x
	   /_/_/_/
	  /_/_/_/
	 /_/_/_/
	z 
	*/

	void Deserialize(string);
	string Serialize();
	void SaveState();
	void DoMethod(CubeRotateMethod);
	void R();
	void Ri();
	void L();
	void Li();
	void B();
	void Bi();
	void D();
	void Di();
	void F();
	void Fi();
	void U();
	void Ui();
	void RotateLeft();
	void RotateRight();
	void RotateUp();
	void RotateDown();
	void RotateClockwise();
	void RotateCounterClockwise();
	bool CheckL();
	bool CheckR();
	bool CheckU();
	bool CheckD();
	bool CheckF();
	bool CheckB();
	bool Check();

	// completely same, not only equivalent
	bool operator==(const Cube&);

	// TODO
	bool Equivalent(const Cube&);

private:
	cube_t oldSubCubes[3][3][3];
};

