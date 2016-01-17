#include "stdafx.h"
#include "Package.h"

void *Package::operator new(size_t size, package_len_t dataLen)
{
	size_t dataSize = static_cast<size_t>(dataLen);
	return ::operator new(sizeof(PackageHeader) + dataSize);
}

void Package::operator delete(void *ptr)
{
	::operator delete(ptr);
}

package_len_t htonpacklen(package_len_t len)
{
	if (sizeof(package_len_t) == 2)
	{
		return static_cast<package_len_t>(htons(len));
	}
	else if (sizeof(package_len_t) == 4)
	{
		return static_cast<package_len_t>(htonl(len));
	}
}

package_len_t ntohpacklen(package_len_t len)
{
	if (sizeof(package_len_t) == 2)
	{
		return static_cast<package_len_t>(ntohs(len));
	}
	else if (sizeof(package_len_t) == 4)
	{
		return static_cast<package_len_t>(ntohl(len));
	}
}