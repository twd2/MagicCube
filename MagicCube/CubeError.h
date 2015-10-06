#pragma once

#include <string>

class CubeError
{
public:
	string why;

	CubeError();
	CubeError(string why) : why(why)
	{}

	~CubeError();
};

