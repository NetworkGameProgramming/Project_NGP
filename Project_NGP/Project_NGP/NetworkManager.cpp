#include "stdafx.h"
#include "NetworkManager.h"

NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager()
{
}

bool NetworkManager::Initialize()
{
	// 윈속 초기화
	if (0 != WSAStartup(MAKEWORD(2, 2), &m_wsa))
		return false;

	// server socket
	m_serversocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
	if (INVALID_SOCKET == m_serversocket)
		return false;


	return true;
}

void NetworkManager::Release()
{
	closesocket(m_serversocket);
	WSACleanup();
}

bool NetworkManager::ConnectToServer(const char* ip)
{
	// 서버 구조체를 양식에 맞게 채움
	ZeroMemory(&m_serverAddr, sizeof(sockaddr_in));
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(SERVER_PORT);
	m_serverAddr.sin_addr.S_un.S_addr = inet_addr(ip);

	// 연결 요청
	int result = connect(m_serversocket, (struct sockaddr*) &m_serverAddr, sizeof(m_serverAddr));

	if (SOCKET_ERROR == result)
	{
		MessageBox(g_hWnd_New, L"접속에 실패하였습니다.\n IP주소나 서버 상태를 확인하여 다시 접속해주세요.", L"Error", MB_OK);
		return false;
	}

	// 로그인 처리
	SPLOGIN info;
	info.type = SP_LOGIN_OK;
	info.id = 0;

	if (-1 == Send(&info, SP_LOGIN_OK, sizeof(SPLOGIN)))
		return false;

	SPLOGIN recvInfo;
	
	if (-1 == Recv(&recvInfo))
		return false;
	
	m_myID = recvInfo.id;

	MessageBox(g_hWnd_New, L"서버에 접속하였습니다!", L"알림", MB_OK);
	return true;
}

bool NetworkManager::SendPlayerInfo(SCENESTATE SceneState, const PLAYERINFO& info)
{
	SPPLAYER PInfo;
	PInfo.id = m_myID;
	PInfo.scene_state = SceneState;
	PInfo.type = SP_PLAYER;
	PInfo.size = sizeof(SPPLAYER);
	PInfo.info = info;
	
	if (-1 == Send(&PInfo, SP_PLAYER, sizeof(SPPLAYER)))
		return false;

	return true;
}

bool NetworkManager::SendAndRecvOtherInfo(char* OutInfo)
{
	if (-1 == Send(OutInfo, SP_OTHERPLAYER, 3))
		return false;

	if (-1 == Recv(OutInfo))
		return false;

	return true;
}

bool NetworkManager::SendAndRecvEvent(EVENTINFO* OutEvInfo)
{
	if (-1 == Send(OutEvInfo, SP_EVENT, 3))
		return false;

	if (-1 == Recv(OutEvInfo))
		return false;

	return true;
}

bool NetworkManager::SendAndRecvMonster(char* OutInfo)
{
	if (-1 == Send(OutInfo, SP_MONSTER, 4))
		return false;

	if (-1 == Recv(OutInfo))
		return false;

	return true;
}

int NetworkManager::Send(void* packet_struct, char type, short size)
{
	// 패킷변환후
	Packing(m_netBuffer, packet_struct, type);

	// 서버로 패킷을 보낸다.
	int byte = send(m_serversocket, m_netBuffer,
		size, 0);

	if (0 == byte)
		return -1;

	return byte;
}

int NetworkManager::Recv(void* OutPacket_struct)
{
	int byte = recv(m_serversocket, m_netBuffer, MAX_BUFFER, 0);

	if (0 == byte)
		return -1;

	// 풀기
	Depacking(OutPacket_struct, m_netBuffer, byte);

	return byte;
}

void NetworkManager::Packing(char* OutPacket, void* packet_struct, char type)
{
	switch (type)
	{
	case SP_LOGIN_OK:
	{
		SPLOGIN s;
		SPLOGIN* ps = reinterpret_cast<SPLOGIN*>(packet_struct);
		s.size = sizeof(SPLOGIN);
		s.type = ps->type;
		s.id = ps->id;
		memcpy(OutPacket, &s, s.size);
	}
		break;
	case SP_PLAYER:
	{
		SPPLAYER s;
		SPPLAYER* ps = reinterpret_cast<SPPLAYER*>(packet_struct);
		s.size = sizeof(SPPLAYER);
		s.type = ps->type;
		s.id = ps->id;
		s.scene_state = ps->scene_state;
		s.info = ps->info;
		memcpy(OutPacket, &s, s.size);
	}
	break;
	case SP_OTHERPLAYER:
	{
		// 다른 플레이어
		short size = 3;
		memcpy(&OutPacket[0], &size, sizeof(short));
		OutPacket[2] = SP_OTHERPLAYER;
	}
	break;
	case SP_MONSTER:
	{
		char* ps = reinterpret_cast<char*>(packet_struct);

		// 다른 플레이어
		short size = 4;
		memcpy(&OutPacket[0], &size, sizeof(short));
		OutPacket[2] = SP_MONSTER;
		OutPacket[3] = ps[3];
	}
	break;
	case SP_EVENT:
	{
		short size = 3;
		memcpy(&OutPacket[0], &size, sizeof(short));
		OutPacket[2] = SP_EVENT;
	}
	break;
	}
}

void NetworkManager::Depacking(void* OutPacket_struct, char* buf, short size)
{
	switch (buf[2])
	{
	case SP_LOGIN_OK:
		memcpy(OutPacket_struct, buf, sizeof(SPLOGIN));
		break;
	case SP_PLAYER:
		memcpy(OutPacket_struct, buf, sizeof(SPPLAYER));
		break;
	case SP_OTHERPLAYER:
		memcpy(OutPacket_struct, buf, size);
		break;
	case SP_MONSTER:
		memcpy(OutPacket_struct, buf, size);
		break;
	case SP_EVENT:
		memcpy(OutPacket_struct, buf, sizeof(EVENTINFO));
		break;
	}
}
