#include "stdafx.h"
#include "utilities.h"

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

void printTime(FILE *fd)
{
	time_t now = time(NULL);
	char tmpBuf[256];
	tm t;
#ifdef _WIN32
	localtime_s(&t, &now);
#else
	t = *localtime(&now);
#endif
	strftime(tmpBuf, sizeof(tmpBuf), "%Y-%m-%d %H:%M:%S", &t);
	fprintf(fd, "[%s]", tmpBuf);
}