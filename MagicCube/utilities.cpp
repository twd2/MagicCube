#include "stdafx.h"
#include "utilities.h"

//http://stackoverflow.com/questions/236129/split-a-string-in-c
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

char toUpper(char c)
{
	return c & ~32;
}

char toLower(char c)
{
	return c | 32;
}

string toUpperString(string str)
{
	transform(str.begin(), str.end(), str.begin(), toUpper);
	return  str;
}

string toLowerString(string str)
{
	transform(str.begin(), str.end(), str.begin(), toLower);
	return  str;
}