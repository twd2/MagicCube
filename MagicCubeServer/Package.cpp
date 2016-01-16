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
