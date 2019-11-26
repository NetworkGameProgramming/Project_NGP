#include "pch.h"
#include "MainServer.h"

MainServer::MainServer()
{
}

MainServer::~MainServer()
{
	Release();
}

bool MainServer::Initialize()
{
	// �����ڵ� �ѱ� ����� ����
	wcout.imbue(std::locale("korean"));
	// ���� �ʱ�ȭ
	if (0 != WSAStartup(MAKEWORD(2, 2), &m_WSAData))
	{
		error_display("���� �ʱ�ȭ ����");
		return false;
	}
		
	// listen ���� ����
	m_listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0,
		WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_listenSocket)
	{ 
		error_display("���� ���� ���� ����");
		return false;
	}
	// ���� �ּ� ���� ����ü
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// ���� �ּ�, ��Ʈ �Ҵ�
	if (0 != ::bind(m_listenSocket, (sockaddr*)&serverAddr,
		sizeof(SOCKADDR_IN)))
	{ 
		error_display("���� ���� ���ε� ����");
		return false;
	}
	// ���� ���� ���� �������� ����
	if (0 != listen(m_listenSocket, SOMAXCONN))
	{ 
		error_display("���� ���� ���� ���� ����");
		return false;
	}
	
	memset(&m_clientAddr, 0, sizeof(SOCKADDR_IN));
	
	// iocp ����
	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if (NULL == g_iocp)
	{ 
		error_display("IOCP ���� ����");
		return false;
	}

	return true;
}

bool MainServer::Running()
{
	SYSTEM_INFO sys_info;
	GetSystemInfo(&sys_info);

	// cpu thread ������ ������ �´�.
	int thread_count = sys_info.dwNumberOfProcessors;

	// cpu thread ������ŭ ����ó���� �ϰڴ�.
	for (int i = 0; i < thread_count; ++i)
	{
		m_vecThread.emplace_back(do_worker);
	}

	int addrLen = sizeof(SOCKADDR_IN);
	g_id = 1;
	while (true)
	{
		m_clientSocket = accept(m_listenSocket, (sockaddr*)&m_clientAddr, &addrLen);
		int user_id = g_id++;

		g_mapClient[user_id] = SOCKET_INFO{};
		g_mapClient[user_id].socket = m_clientSocket;
		g_mapClient[user_id].over_info.wsaBuffer.len = MAX_BUFFER;
		g_mapClient[user_id].over_info.wsaBuffer.buf = g_mapClient[user_id].over_info.buffer;
		g_mapClient[user_id].over_info.is_recv = true;

		printf("[Ŭ���̾�Ʈ ����] ID : %d, IP : %s, PORT : %d, SOCKET : %d\n",  
			user_id, inet_ntoa(m_clientAddr.sin_addr), ntohs(m_clientAddr.sin_port), m_clientSocket);

		memcpy(&g_mapClient[user_id].addr_info, &m_clientAddr, sizeof(SOCKADDR_IN));

		m_flags = 0;
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_clientSocket), g_iocp, user_id, 0);

		int result = WSARecv(m_clientSocket, &g_mapClient[user_id].over_info.wsaBuffer, 
			1, NULL, &m_flags, &(g_mapClient[user_id].over_info.over), NULL);
		if (0 != result)
		{
			int err_no = WSAGetLastError();
			if (WSA_IO_PENDING != err_no)
			{
				error_display("WSARecv �Լ� ����");
				return false;
			}
		}
	}
	return true;
}

void MainServer::Release()
{
	for (auto& t : m_vecThread)
	{
		t.join();
	}
	m_vecThread.clear();
	closesocket(m_listenSocket);
	WSACleanup();
}

void MainServer::error_display(const char * msg)
{
	WCHAR *lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	cout << msg << endl;
	wcout << L"Error : " << lpMsgBuf << endl;
	while (true);
	LocalFree(lpMsgBuf);
}

void MainServer::do_worker()
{
	while (true)
	{
		DWORD byte;
		ULONG key;
		PULONG pKey = &key;
		WSAOVERLAPPED *pOver;

		// ��� �Ϸ� ������ ������ ť�� ���� ������ �´�.
		GetQueuedCompletionStatus(g_iocp, &byte, (PULONG_PTR)pKey,
			&pOver, INFINITE);

		SOCKET clientsocket = g_mapClient[key].socket;

		if (0 == byte)
		{
			closesocket(clientsocket);
			printf("[Ŭ���̾�Ʈ ����] ID : %d, IP : %s, PORT : %d, SOCKET : %d\n",
				key, inet_ntoa(g_mapClient[key].addr_info.sin_addr),
					 ntohs(g_mapClient[key].addr_info.sin_port), clientsocket);
			g_mapClient.erase(clientsocket);
			continue;
		}

		OVERLAPPED_INFO *over_info = reinterpret_cast<OVERLAPPED_INFO*>(pOver);

		if (true == over_info->is_recv)
		{
			// Ŭ���̾�Ʈ���� ���� ��Ŷ�� ó��
			ProcessPacket(key, over_info->buffer);

			// �޾������� ó��
			DWORD flags = 0;
			memset(&over_info->over, 0x00, sizeof(WSAOVERLAPPED));
			WSARecv(clientsocket, &over_info->wsaBuffer, 1, 0, &flags, &over_info->over, NULL);
		}
		else
		{
			// �����Ҷ��� ó��
			// Send�Ҷ� 
			delete over_info;
		}
	}
}

void MainServer::ProcessPacket(int id, void* buf)
{
	char* packet = reinterpret_cast<char*> (buf);

	switch (packet[1])
	{
	case SP_LOGIN_OK:
	break;
	case SP_PLAYER:
	{
		SPPLAYER *PlayerInfo = reinterpret_cast<SPPLAYER*> (buf);
		g_mapClient[id].posX = PlayerInfo->pos_x;
		g_mapClient[id].posY = PlayerInfo->pos_y;
		g_mapClient[id].player_state = PlayerInfo->player_state;
	}
	break;
	case SP_OTHERPLAYER:
	break;
	}

	for (auto& cl : g_mapClient)
	{
		SendProcess(cl.first, id, buf);
	}
}

void MainServer::SendProcess(int send_id, int id, void* buf)
{
	char* packet = reinterpret_cast<char*> (buf);

	switch (packet[1])
	{
	case SP_LOGIN_OK:
	{
		if (send_id != id)
			return;
		SPLOGIN packet;
		packet.id = id;
		packet.size = sizeof(packet);
		packet.type = SP_LOGIN_OK;
		SendPacket(send_id, id, &packet);
	}
	break;
	case SP_PLAYER:
	break;
	case SP_OTHERPLAYER:
	{
		if (send_id != id)
			return;
		char tempBuffer[MAX_BUFFER];

		// ���� ������ �ٸ� Ŭ���̾�Ʈ ������
		int other_client_count = (int)g_mapClient.size() - 1;

		// size
		tempBuffer[0] = sizeof(SPOTHERPLAYERS) * other_client_count + sizeof(char) + sizeof(char);
		// type
		tempBuffer[1] = SP_OTHERPLAYER;

		// size type�� ���� ��Ŷ�� �ּ� ���� ��ġ
		int startAddrPos = 2;
		int count = 0;
		for (auto& c : g_mapClient)
		{
			// �ڽ��� ���� �ʴ´�.
			if (send_id == c.first)
				continue;

			SPOTHERPLAYERS info = SPOTHERPLAYERS{};
			info.id = c.first;
			info.pos_x = c.second.posX;
			info.pos_y = c.second.posY;
			info.player_state = c.second.player_state;

			memcpy((tempBuffer + startAddrPos + sizeof(SPOTHERPLAYERS) * count),
				&info, sizeof(SPOTHERPLAYERS));
			++count;
		}
		SendPacket(send_id, id, &tempBuffer);
	}
	break;
	}
}

void MainServer::SendPacket(int send_id, int id, void* buf)
{
	char* packet = reinterpret_cast<char*>(buf);
	int packet_size = packet[0];

	OVERLAPPED_INFO* send_over = new OVERLAPPED_INFO;
	memset(send_over, 0x00, sizeof(OVERLAPPED_INFO));
	send_over->is_recv = false; // recv �ƴϴ�~
	memcpy(send_over->buffer, packet, packet_size);
	send_over->wsaBuffer.buf = send_over->buffer;
	send_over->wsaBuffer.len = packet_size;
	int result = WSASend(g_mapClient[send_id].socket, &send_over->wsaBuffer, 1, 0, 0, &send_over->over, NULL);
	result = 0;
}
