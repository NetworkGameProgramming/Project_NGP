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
	bool RecvOtherInfo(SPPLAYER* OutInfo);

private:
	// ������
	int Send(void* packet_struct, char type);
	// �ޱ�
	int Recv(void* OutPacket_struct);
	// ��ŷ
	void Packing(char* OutBuf, void* packet_struct, char type);
	// Ǯ��
	void Depacking(void* OutPacket_struct, char* buf);

private:
	WSADATA			m_wsa;
	SOCKET			m_serversocket;
	sockaddr_in		m_serverAddr;

	OVERLAPPEDINFO	m_overlappedInfo = OVERLAPPEDINFO{};
	
	int				m_myID = 0;
};

