#pragma once

#include "CubeSession.h"

class RoomInfo
{
public:
	string Name, Key;
	size_t Capacity;
	list<CubeSession*> Sessions;

	RoomInfo(const RoomInfo&&);

	RoomInfo();
	~RoomInfo();

	bool Enter(CubeSession*);
	void Exit(CubeSession*);

private:

	// prevent copying
	DISALLOW_COPY_AND_ASSIGN(RoomInfo);

	shared_ptr<mutex> ptrMutex;

};

