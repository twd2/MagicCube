#pragma once

#include <mutex>
#include <condition_variable>

class ManualEvent
{
public:

	bool AutoReset = false;

	ManualEvent();
	ManualEvent(bool);
	ManualEvent(bool, bool);
	~ManualEvent();

	void Set();
	void Reset();
	void Wait();

private:
	mutex mtx;
	condition_variable condvar;
	bool set = false;

};

