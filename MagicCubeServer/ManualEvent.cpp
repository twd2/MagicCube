#include "stdafx.h"
#include "ManualEvent.h"


ManualEvent::ManualEvent()
{

}

ManualEvent::ManualEvent(bool set) : set(set)
{

}

ManualEvent::ManualEvent(bool set, bool autoReset) : set(set), AutoReset(autoReset)
{

}

ManualEvent::~ManualEvent()
{
	if (!mtx.try_lock())
	{
		throw "cannot destory while someone is waiting";
	}
	else
	{
		mtx.unlock();
	}

	//AutoReset = false;
	//Set();
	//mtx.lock();
	//mtx.unlock();
}

void ManualEvent::Set()
{
	unique_lock<mutex> lck(mtx);
	set = true;
	condvar.notify_all();
}

void ManualEvent::Reset()
{
	set = false;
}

void ManualEvent::Wait()
{
	unique_lock<mutex> lck(mtx);
	while (!set) condvar.wait(lck);
	if (AutoReset) set = false;
}