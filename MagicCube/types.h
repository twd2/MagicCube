
typedef unsigned int cube_t; //0bMETA DATA ffff bbbb llll rrrr uuuu dddd

#define GET_FRONT(x) ((CubeColor)(((x)&0xF00000)>>20))
#define GET_BACK(x)  ((CubeColor)(((x)&0x0F0000)>>16))
#define GET_LEFT(x)  ((CubeColor)(((x)&0x00F000)>>12))
#define GET_RIGHT(x) ((CubeColor)(((x)&0x000F00)>>8))
#define GET_UP(x)    ((CubeColor)(((x)&0x0000F0)>>4))
#define GET_DOWN(x)  ((CubeColor)(((x)&0x00000F)>>0))

#define SET_FRONT(x) (((x)&0xF)<<20)
#define SET_BACK(x)  (((x)&0xF)<<16)
#define SET_LEFT(x)  (((x)&0xF)<<12)
#define SET_RIGHT(x) (((x)&0xF)<<8)
#define SET_UP(x)    (((x)&0xF)<<4)
#define SET_DOWN(x)  (((x)&0xF)<<0)

#define MAKE_CUBE(f, b, l, r, u, d) (SET_FRONT(f)|SET_BACK(b)|SET_LEFT(l)|SET_RIGHT(r)|SET_UP(u)|SET_DOWN(d))

#define ROTATE_LEFT(x)  MAKE_CUBE(GET_RIGHT(x), GET_LEFT(x), GET_FRONT(x), GET_BACK(x), GET_UP(x), GET_DOWN(x))
#define ROTATE_RIGHT(x) MAKE_CUBE(GET_LEFT(x), GET_RIGHT(x), GET_BACK(x), GET_FRONT(x), GET_UP(x), GET_DOWN(x))
#define ROTATE_UP(x)    MAKE_CUBE(GET_DOWN(x), GET_UP(x), GET_LEFT(x), GET_RIGHT(x), GET_FRONT(x), GET_BACK(x))
#define ROTATE_DOWN(x)  MAKE_CUBE(GET_UP(x), GET_DOWN(x), GET_LEFT(x), GET_RIGHT(x), GET_BACK(x), GET_FRONT(x))
#define ROTATE_CLK(x)   MAKE_CUBE(GET_FRONT(x), GET_BACK(x), GET_DOWN(x), GET_UP(x), GET_LEFT(x), GET_RIGHT(x)) //clockwise
#define ROTATE_CCLK(x)  MAKE_CUBE(GET_FRONT(x), GET_BACK(x), GET_UP(x), GET_DOWN(x), GET_RIGHT(x), GET_LEFT(x)) //counter-clockwise

#define FL_EDGE 0][1][2 //front left edge
#define FR_EDGE 2][1][2 //front right edge
#define FU_EDGE 1][2][2 //front up edge
#define FD_EDGE 1][0][2 //front down edge
#define BL_EDGE 0][1][0 //back left edge
#define BR_EDGE 2][1][0	//back right edge
#define BU_EDGE 1][2][0	//back up edge
#define BD_EDGE 1][0][0	//back down edge
#define UL_EDGE 0][2][1 //up left edge
#define UR_EDGE 2][2][1 //up right edge
#define DL_EDGE 0][0][1 //down left edge
#define DR_EDGE 2][0][1 //down right edge

#define F_CENTRE 1][1][2 //front centre
#define B_CENTRE 1][1][0 //back centre
#define L_CENTRE 0][1][1 //left centre
#define R_CENTRE 2][1][1 //right centre
#define U_CENTRE 1][2][1 //up centre
#define D_CENTRE 1][0][1 //down centre

#define FLU_CORNER 0][2][2 //front left up corner
#define FLD_CORNER 0][0][2 //front left down corner
#define FRU_CORNER 2][2][2 //front right up corner
#define FRD_CORNER 2][0][2 //front right down corner
#define BLU_CORNER 0][2][0 //back left up corner
#define BLD_CORNER 0][0][0 //back left down corner
#define BRU_CORNER 2][2][0 //back right up corner
#define BRD_CORNER 2][0][0 //back right down corner

enum CubeColor
{
	COLOR_UNUSED,
	COLOR_WHITE,
	COLOR_YELLOW,
	COLOR_ORANGE,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_INVALID = 0xF
};

//i means inverse, +9 -> i
enum CubeRotateMethod
{
	NONE,
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	WHOLEX,  //whole up
	WHOLEY,  //whole left
	WHOLEZ,  //whole clockwise
	FRONTi,
	BACKi,
	LEFTi,
	RIGHTi,
	UPi,
	DOWNi,
	WHOLEXi, //whole down
	WHOLEYi, //whole right
	WHOLEZi  //whole counter-clockwise
};