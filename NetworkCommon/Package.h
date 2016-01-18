#pragma once

#include "NetworkConfig.h"
#include "NetworkTypes.h"
#include <event2/event.h>

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
	void operator delete(void*);
};
#pragma pack(pop)

package_len_t htonpacklen(package_len_t);
package_len_t ntohpacklen(package_len_t);