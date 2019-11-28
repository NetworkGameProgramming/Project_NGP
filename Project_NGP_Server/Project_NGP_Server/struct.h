#pragma once
#include <WS2tcpip.h>
#include "define.h"
#include "constant.h"


// 소켓 구조체
struct OVERLAPPED_INFO
{
	WSAOVERLAPPED over;			// OVERLAPPED I/O를 사용하기 위해 필요한 윈도우 구조체
	WSABUF	wsaBuffer;			// 통신을 위해 필요한 버퍼 (length, buffer 구조의 구조체로 되어 있음)
	char	buffer[MAX_BUFFER];	// wsaBuffer에서 buffer는 이 변수의 포인터를 참조 하도록 할것이다.
	bool	is_recv;			// 보내거나 받는다는 것을 알 수 있도록 해주기 위함
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
	SOCKET socket;				// 소켓
	
	// 게임 정보가 들어간다.
	// 플레이어
	PLAYERINFO player_info;
	// 이벤트 큐
	queue<EVENTINFO> event_queue;
	mutex event_lock;
};

// 패킷 구조체
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