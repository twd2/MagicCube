#pragma once

class TcpClient
{
public:
	bool IsAlive;

	TcpClient();
	~TcpClient();

	bool Connect(const string&, short);
	void Close();

	void Wait();

	virtual void Start();

	void Reader();
	void Writer();

	virtual void OnPackage(Package*&);

	void SendPackage(Package*&);
	void SendPackage(string);

	static Package *MakePackage(string&);

protected:

	thread *threadReader = NULL, *threadWriter = NULL;

	evutil_socket_t fd = static_cast<evutil_socket_t>(0);

	queue<Package*> pendingPackages;
	mutex writeLock, queueLock;

	bool bufferedRecv(char*, size_t);
	bool bufferedSend(char*, size_t);

private:

	DISALLOW_COPY_AND_ASSIGN(TcpClient);
};

