#pragma once
class MainServer
{
public:
	MainServer();
	~MainServer();
public:
	bool Initialize();
	bool Running();
	void Release();
private:
	WSADATA			m_WSAData;
	SOCKET			m_listenSocket;
	SOCKET			m_clientSocket;
	SOCKADDR_IN		m_clientAddr;
	DWORD			m_flags;
};

