#pragma once
class SolverError
{
public:
	string why;

	SolverError();
	SolverError(string why) : why(why)
	{}
	~SolverError();
};

