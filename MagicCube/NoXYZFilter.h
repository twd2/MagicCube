#pragma once

#include <map>
#include <vector>
#include "utilities.h"
#include "types.h"
#include "StepFilter.h"

class NoXYZFilter :
	public StepFilter
{
public:
	virtual ~NoXYZFilter() = 0;

	static CubeSteps Filter(CubeSteps&);

private:
	NoXYZFilter();

	static void CombineMapTable(map<CubeRotateMethod, CubeRotateMethod>&, map<CubeRotateMethod, CubeRotateMethod>&);
};

extern map<CubeRotateMethod, map<CubeRotateMethod, CubeRotateMethod> > XYZMapTables;

