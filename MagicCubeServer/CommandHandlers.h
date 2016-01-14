#pragma once

#include "TcpServer.h"

typedef void(*ptrCommandHandler)(TcpServer&, string);

void initHandlers();
void handleCommand(TcpServer&);