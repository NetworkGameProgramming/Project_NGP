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
	// ���� �ʱ�ȭ
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
	// ���� ����ü�� ��Ŀ� �°� ä��
	ZeroMemory(&m_serverAddr, sizeof(sockaddr_in));
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(SERVER_PORT);
	m_serverAddr.sin_addr.S_un.S_addr = inet_addr(ip);

	// ���� ��û
	int result = connect(m_serversocket, (struct sockaddr*) &m_serverAddr, sizeof(m_serverAddr));

	if (SOCKET_ERROR == result)
	{
		MessageBox(g_hWnd_New, L"���ӿ� �����Ͽ����ϴ�.\n IP�ּҳ� ���� ���¸� Ȯ���Ͽ� �ٽ� �������ּ���.", L"Error", MB_OK);
		return false;
	}

	// �α��� ó��
	SPLOGIN info;
	info.type = SP_LOGIN_OK;
	info.id = 0;

	Send(&info, SP_LOGIN_OK);

	SPLOGIN recvInfo;
	
	Recv(&recvInfo);
	
	m_myID = recvInfo.id;

	MessageBox(g_hWnd_New, L"������ �����Ͽ����ϴ�!", L"�˸�", MB_OK);
	return true;
}

bool NetworkManager::SendPlayerInfo(const PLAYERINFO& info)
{
	SPPLAYER PInfo;
	PInfo.id = m_myID;
	PInfo.type = SP_PLAYER;
	PInfo.size = sizeof(SPPLAYER);
	PInfo.info = info;

	Send(&PInfo, SP_PLAYER);

	return true;
}

bool NetworkManager::SendAndRecvOtherInfo(char* OutInfo)
{
	Send(OutInfo, SP_OTHERPLAYER);

	int result = Recv(OutInfo);

	if (0 == result ||
		SP_OTHERPLAYER != OutInfo[1])
		return false;

	return true;
}

bool NetworkManager::SendAndRecvEvent(EVENTINFO* OutEvInfo)
{
	Send(OutEvInfo, SP_EVENT);

	int result = Recv(OutEvInfo);

	if (0 == result ||
		SP_EVENT != OutEvInfo->type)
		return false;

	return true;
}

int NetworkManager::Send(void* packet_struct, char type)
{
	// ��Ŷ��ȯ��
	Packing(m_netBuffer, packet_struct, type);

	// ������ ��Ŷ�� ������.
	int byte = send(m_serversocket, m_netBuffer,
		m_netBuffer[0], 0);

	return byte;
}

int NetworkManager::Recv(void* OutPacket_struct)
{
	int byte = recv(m_serversocket, m_netBuffer, MAX_BUFFER, 0);

	// Ǯ��
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
		s.info = ps->info;
		memcpy(OutPacket, &s, s.size);
	}
	break;
	case SP_OTHERPLAYER:
	{
		// �ٸ� �÷��̾�
		OutPacket[0] = 2;
		OutPacket[1] = SP_OTHERPLAYER;
	}
	break;
	case SP_EVENT:
	{
		OutPacket[0] = 2;
		OutPacket[1] = SP_EVENT;
	}
	break;
	}
}

void NetworkManager::Depacking(void* OutPacket_struct, char* buf, char size)
{
	switch (buf[1])
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
	case SP_EVENT:
		memcpy(OutPacket_struct, buf, sizeof(EVENTINFO));
		break;
	}
}
