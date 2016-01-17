#include "stdafx.h"
#include "RoomInfo.h"

RoomInfo::RoomInfo(const RoomInfo &&ri)
	: Name(move(ri.Name)), Key(move(ri.Key)), Capacity(ri.Capacity),
	  Sessions(move(ri.Sessions)), ptrMutex(move(ri.ptrMutex))
{

}

RoomInfo::RoomInfo()
	: ptrMutex(new mutex)
{

}


RoomInfo::~RoomInfo()
{

}

bool RoomInfo::Enter(CubeSession *sess)
{
	unique_lock<mutex> lck(*ptrMutex);

	if (Sessions.size() >= Capacity)
	{
		return false;
	}

	Sessions.push_back(sess);
	sess->iterInRoom = Sessions.end();
	--sess->iterInRoom;

	return true;
}

void RoomInfo::Exit(CubeSession *sess)
{
	unique_lock<mutex> lck(*ptrMutex);
	Sessions.erase(sess->iterInRoom);
}
