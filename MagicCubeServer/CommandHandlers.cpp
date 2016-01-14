#include "stdafx.h"
#include "CommandHandlers.h"

map<string, ptrCommandHandler> handlers;

void addHandler(string cmd, ptrCommandHandler handler)
{
	handlers[cmd] = handler;
}

ptrCommandHandler delHandler(string cmd)
{
	ptrCommandHandler oldHandler = handlers[cmd];
	handlers[cmd] = NULL;
	return oldHandler;
}

void handleCommand(TcpServer &server)
{
	string cmd, args;
	while (getline(cin, cmd) && server.IsRunning)
	{
		size_t i = cmd.find(' ');
		args = "";
		if (i != string::npos)
		{
			args = cmd.substr(i + 1);
			cmd = cmd.substr(0, i);
		}

		cmd = toLowerString(cmd);

		if (handlers[cmd])
		{
			handlers[cmd](server, args);
		}
		else
		{
			log_normal("Unhandled command %s", cmd.c_str());
		}
	}
}

void initHandlers()
{

}