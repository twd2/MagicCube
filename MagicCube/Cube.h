#pragma once
class Cube
{
public:
	Cube();
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

	void SaveState();
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
	void RotateClk();
	void RotateCClk();

private:
	cube_t oldSubCubes[3][3][3];
};

