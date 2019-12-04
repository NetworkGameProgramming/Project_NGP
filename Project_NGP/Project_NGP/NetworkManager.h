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
	const int& GetMyId() { return m_myID; }

public:
	bool ConnectToServer(const char* ip);
	bool SendPlayerInfo(SCENESTATE SceneState, const PLAYERINFO& PInfo);
	bool SendAndRecvOtherInfo(char* OutInfo);
	bool SendAndRecvEvent(EVENTINFO* OutEvInfo);
	bool SendAndRecvMonster(char* OutInfo);
	bool SendHitInfo(int monster_id, int damage);
	bool SendGoNextSceneInfo(SCENESTATE nextState, SCENESTATE curState);
public:
	void AttackCollisionForNetwork(ObjectManager::MAPOBJ* TargetList, ObjectManager::MAPOBJ* SkillList);

private:
	// ���ŷ ������
	int Send(void* packet_struct, char type, short size);
	// ���ŷ �ޱ�
	int Recv(void* OutPacket_struct);
	// ��ŷ
	void Packing(char* OutBuf, void* packet_struct, char type);
	// Ǯ��
	void Depacking(void* OutPacket_struct, char* buf, short size = 0);

private:
	WSADATA			m_wsa;
	SOCKET			m_serversocket;
	sockaddr_in		m_serverAddr;

	char			m_netBuffer[MAX_BUFFER];
	
	int				m_myID = 0;
};

