#pragma once

#define SERVER_PORT 3500

// Ŭ���̾�Ʈ ��Ŷ ó��
#define SP_LOGIN_OK			1		// �α���
#define SP_PLAYER			2		// �÷��̾� ��ġ
#define SP_OTHERPLAYER		3
#define SP_EVENT			99		// Event



// �ִ� ���� ���� ũ��
constexpr int MAX_BUFFER = 1024;


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