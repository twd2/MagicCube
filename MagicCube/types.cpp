#include "stdafx.h"
#include "types.h"

map<CubeRotateMethod, string> CubeRotateMethodName = 
{
	{ROTATE_NONE   , "NONE"},
	{ROTATE_FRONT  , "F"   },
	{ROTATE_BACK   , "B"   },
	{ROTATE_LEFT   , "L"   },
	{ROTATE_RIGHT  , "R"   },
	{ROTATE_UP     , "U"   },
	{ROTATE_DOWN   , "D"   },
	{ROTATE_WHOLEX , "X"   },
	{ROTATE_WHOLEY , "Y"   },
	{ROTATE_WHOLEZ , "Z"   },
	{ROTATE_NONEi  , "NONE"},
	{ROTATE_FRONTi , "Fi"  },
	{ROTATE_BACKi  , "Bi"  },
	{ROTATE_LEFTi  , "Li"  },
	{ROTATE_RIGHTi , "Ri"  },
	{ROTATE_UPi    , "Ui"  },
	{ROTATE_DOWNi  , "Di"  },
	{ROTATE_WHOLEXi, "Xi"  },
	{ROTATE_WHOLEYi, "Yi"  },
	{ROTATE_WHOLEZi, "Zi"  }
};

map<CubeRotateMethod, string> CubeRotateMethodEnumName =
{
	{ ROTATE_NONE   , "ROTATE_NONE" },
	{ ROTATE_FRONT  , "ROTATE_FRONT" },
	{ ROTATE_BACK   , "ROTATE_BACK" },
	{ ROTATE_LEFT   , "ROTATE_LEFT" },
	{ ROTATE_RIGHT  , "ROTATE_RIGHT" },
	{ ROTATE_UP     , "ROTATE_UP" },
	{ ROTATE_DOWN   , "ROTATE_DOWN" },
	{ ROTATE_WHOLEX , "ROTATE_WHOLEX" },
	{ ROTATE_WHOLEY , "ROTATE_WHOLEY" },
	{ ROTATE_WHOLEZ , "ROTATE_WHOLEZ" },
	{ ROTATE_NONEi  , "ROTATE_NONEi" },
	{ ROTATE_FRONTi , "ROTATE_FRONTi" },
	{ ROTATE_BACKi  , "ROTATE_BACKi" },
	{ ROTATE_LEFTi  , "ROTATE_LEFTi" },
	{ ROTATE_RIGHTi , "ROTATE_RIGHTi" },
	{ ROTATE_UPi    , "ROTATE_UPi" },
	{ ROTATE_DOWNi  , "ROTATE_DOWNi" },
	{ ROTATE_WHOLEXi, "ROTATE_WHOLEXi" },
	{ ROTATE_WHOLEYi, "ROTATE_WHOLEYi" },
	{ ROTATE_WHOLEZi, "ROTATE_WHOLEZi" }
};

map<string, CubeRotateMethod> NameToCubeRotateMethod =
{
	{"NONE", ROTATE_NONE   },
	{"F"   , ROTATE_FRONT  },
	{"B"   , ROTATE_BACK   },
	{"L"   , ROTATE_LEFT   },
	{"R"   , ROTATE_RIGHT  },
	{"U"   , ROTATE_UP     },
	{"D"   , ROTATE_DOWN   },
	{"X"   , ROTATE_WHOLEX },
	{"Y"   , ROTATE_WHOLEY },
	{"Z"   , ROTATE_WHOLEZ },
	{"NONE", ROTATE_NONEi  },
	{"Fi"  , ROTATE_FRONTi },
	{"Bi"  , ROTATE_BACKi  },
	{"Li"  , ROTATE_LEFTi  },
	{"Ri"  , ROTATE_RIGHTi },
	{"Ui"  , ROTATE_UPi    },
	{"Di"  , ROTATE_DOWNi  },
	{"Xi"  , ROTATE_WHOLEXi},
	{"Yi"  , ROTATE_WHOLEYi},
	{"Zi"  , ROTATE_WHOLEZi}
};
