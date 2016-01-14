#pragma once

#include "TcpServer.h"

typedef void(*CommandHandler)(TcpServer&, string&);

void initHandlers();
void handleCommand(TcpServer&);