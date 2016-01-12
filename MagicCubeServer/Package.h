#pragma once

#include "Config.h"

#pragma pack(push, 1)
struct PackageHeader
{
	char magic[sizeof(MAGIC_MARK) - 1]; // always 'M', 'C'
	package_len_t length;
};

struct Package
{
	PackageHeader header;
	char data[0];
};
#pragma pack(pop)