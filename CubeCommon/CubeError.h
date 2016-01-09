#pragma once

#include <string>

class CubeError
{
public:
	string what;

	CubeError();
	CubeError(string what) : what(what)
	{}

	~CubeError();
};

