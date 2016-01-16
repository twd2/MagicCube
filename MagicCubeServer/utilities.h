#pragma once

#include "RoomInfo.h"
#include "TcpServer.h"
#include "CubeServer.h"

bool endsWith(const string&, const string&);
void printTime(FILE*);
void setLogFile(string);
Document loadConfigObj(string);
size_t getSizeT(Value&);
void configServer(TcpServer&, Value&);
vector<RoomInfo> loadRooms(Value&);
void configRooms(CubeServer&, vector<RoomInfo>&);