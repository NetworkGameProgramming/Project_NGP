#pragma once

class Scene;
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
	// 서버 통신 처리
	static void do_worker();
	// 씬 처리
	static void do_scene();
	static void ProcessPacket(int id, void* buf, int recv_byte);
	static void SendProcess(int send_id, void* buf);
	static void SendPacket(int send_id, void* buf, int size);
private:
	WSADATA			m_WSAData;
	SOCKET			m_listenSocket;
	SOCKET			m_clientSocket;
	SOCKADDR_IN		m_clientAddr;
	DWORD			m_flags;

	vector<thread>  m_vecThread;
	static vector<Scene*> m_vecScene;
};

