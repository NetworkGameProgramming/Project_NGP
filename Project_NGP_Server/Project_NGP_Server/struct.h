#pragma once
#include <WS2tcpip.h>
#include "define.h"
#include "constant.h"


// ���� ����ü
struct OVERLAPPED_INFO
{
	WSAOVERLAPPED over;			// OVERLAPPED I/O�� ����ϱ� ���� �ʿ��� ������ ����ü
	WSABUF	wsaBuffer;			// ����� ���� �ʿ��� ���� (length, buffer ������ ����ü�� �Ǿ� ����)
	char	buffer[MAX_BUFFER];	// wsaBuffer���� buffer�� �� ������ �����͸� ���� �ϵ��� �Ұ��̴�.
	bool	is_recv;			// �����ų� �޴´ٴ� ���� �� �� �ֵ��� ���ֱ� ����
};

struct SOCKET_INFO
{
	OVERLAPPED_INFO over_info;
	SOCKET socket;				// ����
	int id;						// Ŭ���̾�Ʈ���� id�� ���´�.
	
	// ���� ������ ����.
	int posX, posY;
};

// ��Ŷ ����ü
#pragma pack(push, 1)

// Network Packet
typedef struct ServerPacketTypeLogin
{
	char size;
	char type;
	int id;
}SPLOGIN;

typedef struct ClientPacketTypePos
{
	char size;
	char type;
	int id;
	short x, y;
}SPPOS;

typedef struct ServerPacketEnd
{
	char size;
	char type;
	int id;
}SPEND;

#pragma pack(pop)