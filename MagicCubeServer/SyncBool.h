#pragma once
class SyncBool
{
public:
	SyncBool();
	
	SyncBool(bool v) : value(v)
	{

	}

	~SyncBool();

	SyncBool &operator=(const SyncBool&);
	SyncBool &operator=(const bool&);
	bool operator==(const SyncBool&);
	bool operator==(bool);

private:
	bool value;
	mutex mtx;

	SyncBool(const SyncBool&) = delete;
};

