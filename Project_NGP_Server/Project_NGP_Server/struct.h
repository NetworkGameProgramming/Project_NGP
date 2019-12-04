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

enum EventState
{
	EV_PUTOTHERPLAYER,
	EV_END,
	EV_CHAT,
	EV_NONE
};

#pragma pack(push, 1)
typedef struct EventInfo
{
	short size;
	char type;
	int id;
	//
	char scene_state;
	char event_state;
	//
	char chat_buffer[128];
	char chat_size;
}EVENTINFO;

typedef struct PlayerInfo
{
	short	pos_x, pos_y;
	int		player_state;
	char	player_dir;
}PLAYERINFO;

typedef struct MonsterInfo
{
	char	monster_type;
	short	pos_x, pos_y;
	int		monster_state;
	char	monster_dir;
}MONSTERINFO;

#pragma pack(pop)

struct SOCKET_INFO
{
	OVERLAPPED_INFO over_info;
	SOCKADDR_IN		addr_info;
	SOCKET socket;				// ����
	
	// ���� ������ ����.
	// �÷��̾�
	char scene_state;
	PLAYERINFO player_info;
	// �̺�Ʈ ť
	queue<EVENTINFO> event_queue;
	mutex event_lock;
};

///////////////////////////////////////////////////////////////
#pragma pack(push, 1)
// Network Packet
typedef struct ServerPacketTypeLogin
{
	short size;
	char type;
	int id;
}SPLOGIN;

typedef struct ServerPacketTypePlayer
{
	short size;
	char type;
	int id;
	char scene_state;
	PLAYERINFO info;
}SPPLAYER;

typedef struct ServerPacketOtherPlayers
{
	int id;
	char scene_state;
	PLAYERINFO info;
}SPOTHERPLAYERS;

typedef struct ServerPacketMonster
{
	int monster_id;
	MONSTERINFO info;
}SPMONSTER;

typedef struct ServerPacketHit
{
	short size;
	char type;
	int id;
	int monster_id;
	int damage;
}SPHIT;

typedef struct ServerPacketGoNext
{
	short size;
	char type;
	int id;
	char cur_scene_state;
	char next_scene_state;
}SPGONEXT;

typedef struct ServerPacketEnd
{
	short size;
	char type;
	int id;
	char scene_state;
}SPEND;

#pragma pack(pop)

////////////////////////////////////////////////////////////////
// �ȼ� ����
typedef struct _tagPixel24
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}PIXEL24;

typedef struct _tagPixelColliderInfo
{
	vector<PIXEL24>		vecPixel;
	UINT				Width;
	UINT				Height;
	PIXEL24				CollPixel;
	PIXEL24				TurnPixel;
}PIXELCOLLIDERINFO;

// ���� ������Ʈ ����
typedef struct GameObjectInfo
{
	int Pos_X;
	int Pos_Y;
	int Size_Width;
	int Size_Height;
}GAMEOBJINFO;

