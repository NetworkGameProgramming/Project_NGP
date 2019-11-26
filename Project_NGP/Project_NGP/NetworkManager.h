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
	bool SendPlayerInfo(short pos_x, short pos_y, int sprite_state);
	
	bool SendAndRecvOtherInfo(char* OutInfo);

private:
	// 보내기
	int Send(void* packet_struct, char type);
	// 받기
	int Recv(void* OutPacket_struct);
	// 블로킹 보내기
	int Send_Blocking(void* packet_struct, char type);
	// 블로킹 받기
	int Recv_Blocking(void* OutPacket_struct);
	// 패킹
	void Packing(char* OutBuf, void* packet_struct, char type);
	// 풀기
	void Depacking(void* OutPacket_struct, char* buf, char size = 0);

private:
	WSADATA			m_wsa;
	SOCKET			m_serversocket;
	sockaddr_in		m_serverAddr;

	OVERLAPPEDINFO	m_overlappedInfo = OVERLAPPEDINFO{};
	
	int				m_myID = 0;
};

