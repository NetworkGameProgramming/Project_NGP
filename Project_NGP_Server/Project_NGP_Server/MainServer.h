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
	void error_display(const char *msg);
	static void do_worker();
	static void ProcessPacket(int id, void* buf);
	static void SendPacket(int id, void* buf);
private:
	WSADATA			m_WSAData;
	SOCKET			m_listenSocket;
	SOCKET			m_clientSocket;
	SOCKADDR_IN		m_clientAddr;
	DWORD			m_flags;

	vector<thread>  m_vecThread;
};

