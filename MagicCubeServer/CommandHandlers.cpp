#include "stdafx.h"
#include "CommandHandlers.h"

map<string, CommandHandler> handlers;

void addHandler(string cmd, CommandHandler handler)
{
	handlers[cmd] = handler;
}

CommandHandler delHandler(string cmd)
{
	CommandHandler oldHandler = handlers[cmd];
	handlers[cmd] = NULL;
	return oldHandler;
}

void handleCommand(TcpServer &server)
{
	string cmd, args;

	printf("Server> ");
	while (server.IsRunning && getline(cin, cmd))
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
			printf("Unhandled command %s\n", cmd.c_str());
		}
		printf("Server> ");
	}
}

void exitHandler(TcpServer &server, string &args)
{
	for (auto &sess : server.Sessions)
	{
			sess->SendPackage("Server is closing."); // TODO: change message
			sess->FlushAndClose();
	}
	while (server.Sessions.size() > 0);
	server.Stop();
}

void msgHandler(TcpServer &server, string &args)
{
	for (auto &sess : server.Sessions)
	{
		sess->SendPackage(args);
	}
}

void initHandlers()
{
#define HAND(x) addHandler(#x, x##Handler)
	HAND(exit);
	HAND(msg);
#undef HAND
}