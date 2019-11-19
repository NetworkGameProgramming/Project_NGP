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

	for (int i = 0; i < thread_count; ++i)
	{
		m_vecThread.emplace_back(do_worker);
	}

	int addrLen = sizeof(SOCKADDR_IN);
	g_id = 0;
	while (true)
	{
		m_clientSocket = accept(m_listenSocket, (sockaddr*)&m_clientAddr, &addrLen);
		int user_id = g_id++;

		g_mapClient[user_id] = SOCKET_INFO{};
		g_mapClient[user_id].socket = m_clientSocket;
		g_mapClient[user_id].over_info.wsaBuffer.len = MAX_BUFFER;
		g_mapClient[user_id].over_info.wsaBuffer.buf = g_mapClient[user_id].over_info.buffer;
		g_mapClient[user_id].over_info.is_recv = true;

		m_flags = 0;
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_clientSocket), g_iocp, user_id, 0);

		// ���� �ʱ�ȭ

		// �ٸ� Ŭ���̾�Ʈ���Ե� �˸�

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
			g_mapClient.erase(clientsocket);
			continue;
		}

		OVERLAPPED_INFO *over_info = reinterpret_cast<OVERLAPPED_INFO*>(pOver);

		if (true == over_info->is_recv)
		{
			// �޾������� ó��
		}
		else
		{
			// �����Ҷ��� ó��
		}
	}
}
