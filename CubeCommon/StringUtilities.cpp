#include "stdafx.h"
#include "StringUtilities.h"

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

bool endsWith(const string &fullString, const string &ending)
{
	if (fullString.length() >= ending.length())
	{
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else
	{
		return false;
	}
}