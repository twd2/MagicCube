#pragma once

class SolverError
{
public:
	string what;

	SolverError();
	SolverError(string what) : what(what)
	{}
	~SolverError();
};

