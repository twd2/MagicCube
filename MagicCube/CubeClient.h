#pragma once
#include "TcpClient.h"

class CubeClient :
	public TcpClient
{
public:

	typedef void (CubeClient::*CommandHandlerPtr)(Value&);

	bool Succeeded = false;
	SessionErrorType LastErrorCode = SESSIONERROR_UNKNOWN;
	string LastErrorMessage;

	CubeClient();
	~CubeClient();

	void Start();
	void Close();
	bool Auth(string);

	void OnPackage(Package*&);

private:

	map<string, CommandHandlerPtr> commandHandlers;
	void initCommandHandlers();

	ManualEvent ackEvent, responseEvent;

	thread *threadHeartbeat = NULL;

	void handleCommandAndAck(Document&);

	void heartbeat();
};

