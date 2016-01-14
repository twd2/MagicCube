#include "stdafx.h"
#include "utilities.h"

// http://stackoverflow.com/questions/236129/split-a-string-in-c
vector<string> &split(const string &s, char delim, vector<string> &elems)
{
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

vector<string> split(const string &s, char delim)
{
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

char toUpper(char c)
{
	if (c < 'a' || c > 'z') return c;
	return c & ~32;
}

char toLower(char c)
{
	if (c < 'A' || c > 'Z') return c;
	return c | 32;
}

string toUpperString(string str)
{
	transform(str.begin(), str.end(), str.begin(), toUpper);
	return str;
}

string toLowerString(string str)
{
	transform(str.begin(), str.end(), str.begin(), toLower);
	return str;
}

string stepsToString(CubeSteps &steps, char delim)
{
	string r;
	ptrdiff_t size = steps.size();
	for (ptrdiff_t i = 0; i < size; ++i)
	{
		r += CubeRotateMethodName[steps[i]] + delim;
	}
	return r;
}

void printError(CubeError err)
{
	if (err.what == "")
	{
		fprintf(stderr, "CubeError\n");
	}
	else
	{
		fprintf(stderr, "CubeError: %s\n", err.what.c_str());
	}
}

void randomCube(Cube &cube)
{
	for (int i = 0; i < rand() % 1000 + 1; ++i)
	{
		CubeRotateMethod method = (CubeRotateMethod)((rand() % 19) + 1);
		cube.DoMethod(method);
	}
}

string randomCube()
{
	Cube cube;
	randomCube(cube);
	return cube.Serialize();
}

CubeRotateMethod inverse(CubeRotateMethod m)
{
	if (m < ROTATE_NONEi)
	{
		return (CubeRotateMethod)(m + (ROTATE_NONEi - ROTATE_NONE)); //inverse
	}
	else
	{
		return (CubeRotateMethod)(m - (ROTATE_NONEi - ROTATE_NONE)); //inverse
	}
}

void copySteps(CubeSteps &src, CubeSteps &dest)
{
	size_t size = src.size();
	for (ptrdiff_t i = 0; i < (ptrdiff_t)size; ++i)
	{
		if (src[i] != ROTATE_NONE && src[i] != ROTATE_NONEi)
			dest.push_back(src[i]);
	}
}

bool isWholeRotate(CubeRotateMethod m)
{
	return (m >= ROTATE_WHOLEX && m <= ROTATE_WHOLEZ) || (m >= ROTATE_WHOLEXi && m <= ROTATE_WHOLEZi);
}

// This table was written by hand, so if you find any mistake, just let me know :)
int format1ToFormat2Table[FORMAT1_LENGTH] = {
	-1, 17, 24, -1, -1, 51, -1, 16, -1, -1, -1, 52, -1, 15, -1, 35, -1, 53, //(0, 0, 0) ~ (2, 0, 0)
	-1, 14, 21, -1, -1, -1, -1, 13, -1, -1, -1, -1, -1, 12, -1, 32, -1, -1, //(0, 1, 0) ~ (2, 1, 0)
	-1, 11, 18, -1, 36, -1, -1, 10, -1, -1, 37, -1, -1,  9, -1, 29, 38, -1, //(0, 2, 0) ~ (2, 2, 0)
	-1, -1, 25, -1, -1, 48, -1, -1, -1, -1, -1, 49, -1, -1, -1, 34, -1, 50, //(0, 0, 1) ~ (2, 0, 1)
	-1, -1, 22, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 31, -1, -1, //(0, 1, 1) ~ (2, 1, 1)
	-1, -1, 19, -1, 39, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, 28, 41, -1, //(0, 2, 1) ~ (2, 2, 1)
	 6, -1, 26, -1, -1, 45,  7, -1, -1, -1, -1, 46,  8, -1, -1, 33, -1, 47, //(0, 0, 2) ~ (2, 0, 2)
	 3, -1, 23, -1, -1, -1,  4, -1, -1, -1, -1, -1,  5, -1, -1, 30, -1, -1, //(0, 1, 2) ~ (2, 1, 2)
	 0, -1, 20, -1, 42, -1,  1, -1, -1, -1, 43, -1,  2, -1, -1, 27, 44, -1  //(0, 2, 2) ~ (2, 2, 2)
};

int format2ToFormat1Table[FORMAT2_LENGTH] = {-1};

void makeTable2()
{
	for (int i = 0; i < FORMAT1_LENGTH; ++i)
	{
		if (format1ToFormat2Table[i] >= 0) format2ToFormat1Table[format1ToFormat2Table[i]] = i;
	}
}

string convertFromFormat2(char *f2)
{
	string f1(FORMAT1_LENGTH, '-');
	for (int i = 0; i < FORMAT1_LENGTH; ++i)
	{
		if (format1ToFormat2Table[i] >= 0)
		{
			f1[i] = f2[format1ToFormat2Table[i]];
		}
	}
	return f1;
}

char *convertToFormat2(string f1)
{
	if (format2ToFormat1Table[0] == -1)
	{
		makeTable2();
	}

	char *f2 = new char[FORMAT2_LENGTH];
	for (int i = 0; i < FORMAT2_LENGTH; ++i)
	{
		f2[i] = f1[format2ToFormat1Table[i]];
	}
	return f2;
}
