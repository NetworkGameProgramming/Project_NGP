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

#pragma pack(push, 1)
enum EventState
{
	EV_PUTOTHERPLAYER,
	EV_END,
	EV_NONE
};

typedef struct EventInfo
{
	char size;
	char type;
	int id;
	EventState state;
}EVENTINFO;

typedef struct PlayerInfo
{
	short	pos_x, pos_y;
	int		player_state;
	char	player_dir;
}PLAYERINFO;

#pragma pack(pop)

struct SOCKET_INFO
{
	OVERLAPPED_INFO over_info;
	SOCKADDR_IN		addr_info;
	SOCKET socket;				// ����
	
	// ���� ������ ����.
	// �÷��̾�
	PLAYERINFO player_info;
	// �̺�Ʈ ť
	queue<EVENTINFO> event_queue;
	mutex event_lock;
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

typedef struct ClientPacketTypePlayer
{
	char size;
	char type;
	int id;
	PLAYERINFO info;
}SPPLAYER;

typedef struct ClientPacketOtherPlayers
{
	int id;
	PLAYERINFO info;
}SPOTHERPLAYERS;

typedef struct ServerPacketEnd
{
	char size;
	char type;
	int id;
}SPEND;

#pragma pack(pop)