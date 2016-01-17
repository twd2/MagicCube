#pragma once

#include "RoomInfo.h"
#include "TcpServer.h"
#include "CubeServer.h"

void printTime(FILE*);
void setLogFile(string);
Document loadConfigObj(string);
size_t getSizeT(Value&);
void configServer(TcpServer&, Value&);
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