#include "stdafx.h"
#include "CubeSession.h"

#ifdef ENABLE_IPV4
CubeSession::CubeSession(TcpServer &server, sockaddr_in addr, evutil_socket_t fd)
	: Session(server, addr, fd)
{

}
#endif

#ifdef ENABLE_IPV6
CubeSession::CubeSession(TcpServer &server, sockaddr_in6 addr, evutil_socket_t fd)
	: Session(server, addr, fd)
{

}
#endif

CubeSession::~CubeSession()
{

}

void CubeSession::OnPackage(Package *&pack)
{
	if (!pack || pack->Header.DataLength == 0) return;
	pack->Data[pack->Header.DataLength - 1] = '\0';
	log_debug("on package (fd = %u): %s", static_cast<unsigned int>(fd), pack->Data);

	Document doc;
	doc.Parse(pack->Data);
	if (!doc.IsObject())
		return;

	SendError(_SE(ERROR_AUTH));
	// TODO: package

	delete pack;
	pack = NULL;
}

void CubeSession::SendError(SessionErrorType errorCode)
{
	log_debug("sending error (fd = %u): %d", static_cast<unsigned int>(fd), errorCode);

	int code = static_cast<int>(errorCode);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);

	writer.StartObject();

	writer.String("error");
	writer.Int(errorCode);

	writer.String("message");

	if (code > SESSIONERROR_UNKNOWN) // user facing error
	{
		writer.String(UserFacingErrorMessage[_UE(errorCode)].c_str());
	}
	else
	{
		writer.String(SessionErrorMessage[errorCode].c_str());
	}

	writer.EndObject();

	SendPackage(buffer.GetString());

	FlushAndClose();
}