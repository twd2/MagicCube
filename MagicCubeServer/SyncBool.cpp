#include "stdafx.h"
//#include "SyncBool.h"
//
//SyncBool::SyncBool()
//{
//
//}
//
//
//SyncBool::~SyncBool()
//{
//
//}
//
//SyncBool &SyncBool::operator=(const SyncBool &b)
//{
//	unique_lock<mutex> lck(mtx);
//	value = b == true;
//
//	return *this;
//}
//
//SyncBool &SyncBool::operator=(const bool &b)
//{
//	unique_lock<mutex> lck(mtx);
//	value = b;
//
//	return *this;
//}
//
//bool SyncBool::operator==(const SyncBool &b)
//{
//	if (&b == this) return true;
//	unique_lock<mutex> lck(mtx);
//	return b == value;
//}
//
//bool SyncBool::operator==(bool b)
//{
//	unique_lock<mutex> lck(mtx);
//	return b == value;
//}
