#pragma once

#include "RoomInfo.h"
#include "TcpServer.h"
#include "CubeServer.h"

void printTime(FILE*);
void setLogFile(string);
Document loadConfigObj(string);
size_t getSizeT(Value&);
void configServer(CubeServer&, Value&);
vector<RoomInfo> loadRooms(Value&);
void configRooms(CubeServer&, vector<RoomInfo>&);

template<typename K, typename V>
V getWithDefault(const map <K, V> &m, const K &key, const V &defval)
{
	typename map<K, V>::const_iterator it = m.find(key);
	if (it == m.end())
	{
		return defval;
	}
	else
	{
		return it->second;
	}
}

template<typename K, typename V>
bool contains(const map <K, V> &m, const K &key)
{
	typename map<K, V>::const_iterator it = m.find(key);
	return it != m.end();
}

// check document's props and type. string: str, int: int, uint: uint, int64: int64, uint64: uint64, float: float, bool: bool, null: null, array: array, object, obj
bool checkObj(Value&, size_t, ...);