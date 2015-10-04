
typedef unsigned int cube_t; //0bMETA DATA ffff bbbb llll rrrr uuuu dddd

#define GET_FRONT(x) (((x)&0xF00000)>>20)
#define GET_BACK(x)  (((x)&0x0F0000)>>16)
#define GET_LEFT(x)  (((x)&0x00F000)>>12)
#define GET_RIGHT(x) (((x)&0x000F00)>>8)
#define GET_UP(x)    (((x)&0x0000F0)>>4)
#define GET_DOWN(x)  (((x)&0x00000F)>>0)

#define SET_FRONT(x) (((x)&0xF)<<20)
#define SET_BACK(x)  (((x)&0xF)<<16)
#define SET_LEFT(x)  (((x)&0xF)<<12)
#define SET_RIGHT(x) (((x)&0xF)<<8)
#define SET_UP(x)    (((x)&0xF)<<4)
#define SET_DOWN(x)  (((x)&0xF)<<0)

#define MAKE_CUBE(f,b,l,r,u,d) (SET_FRONT(f)|SET_BACK(b)|SET_LEFT(l)|SET_RIGHT(r)|SET_UP(u)|SET_DOWN(d))

#define ROTATE_LEFT(x)  MAKE_CUBE(GET_RIGHT(x), GET_LEFT(x), GET_FRONT(x), GET_BACK(x), GET_UP(x), GET_DOWN(x))
#define ROTATE_RIGHT(x) MAKE_CUBE(GET_LEFT(x), GET_RIGHT(x), GET_BACK(x), GET_FRONT(x), GET_UP(x), GET_DOWN(x))
#define ROTATE_UP(x)    MAKE_CUBE(GET_DOWN(x), GET_UP(x), GET_LEFT(x), GET_RIGHT(x), GET_FRONT(x), GET_BACK(x))
#define ROTATE_DOWN(x)  MAKE_CUBE(GET_UP(x), GET_DOWN(x), GET_LEFT(x), GET_RIGHT(x), GET_BACK(x), GET_FRONT(x))
#define ROTATE_CLK(x)   MAKE_CUBE(GET_FRONT(x), GET_BACK(x), GET_DOWN(x), GET_UP(x), GET_LEFT(x), GET_RIGHT(x)) //Ë³Ê±Õë
#define ROTATE_CCLK(x)  MAKE_CUBE(GET_FRONT(x), GET_BACK(x), GET_UP(x), GET_DOWN(x), GET_RIGHT(x), GET_LEFT(x)) //ÄæÊ±Õë

enum cube_color
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
