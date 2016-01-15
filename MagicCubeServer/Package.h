#pragma once

#include "Config.h"

#pragma pack(push, 1)
struct PackageHeader
{
	char Magic[sizeof(MAGIC_MARK) - 1]; // always MAGIC_MARK without '\0'
	package_len_t DataLength;
};

struct Package
{
	PackageHeader Header;
	char Data[0];

	void *operator new(size_t, void*) = delete;

	// suger for variable data size
	void *operator new(size_t, package_len_t);
};
#pragma pack(pop)