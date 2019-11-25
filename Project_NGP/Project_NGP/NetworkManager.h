#pragma once

class NetworkManager : public Singleton<NetworkManager>
{
public:
	NetworkManager();
	virtual ~NetworkManager();

public:
	bool Initialize();
	void Release();

public:
	bool ConnectToServer(const char* ip);

public:
	// 보내기
	int Send(void* packet_struct, char type);
	// 받기
	int Recv(void* OutPacket_struct);

public:
	// 패킹
	void Packing(char* OutBuf, void* packet_struct, char type);
	// 풀기
	void Depacking(void* OutPacket_struct, char* buf);

private:
	WSADATA			m_wsa;
	SOCKET			m_serversocket;
	sockaddr_in		m_serverAddr;

	OVERLAPPEDINFO	m_overlappedInfo = OVERLAPPEDINFO{};
	
	int				m_myID = 0;
};

