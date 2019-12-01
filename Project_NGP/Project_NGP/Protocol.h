#pragma once

#define SERVER_PORT 3500

// 클라이언트 패킷 처리
#define SP_LOGIN_OK			1		// 로그인
#define SP_PLAYER			2		// 플레이어 위치
#define SP_OTHERPLAYER		3
#define SP_MONSTER			4
#define SP_HIT  			5
#define SP_EVENT			99		// Event



// 최대 보낼 버퍼 크기
constexpr int MAX_BUFFER = 1024;

enum EventState
{
	EV_PUTOTHERPLAYER,
	EV_END,
	EV_NONE
};


#pragma pack(push, 1)
typedef struct EventInfo
{
	short size;
	char type;
	int id;
	char scene_state;
	char event_state;
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

// Network Packet
typedef struct ServerPacketTypeLogin
{
	short size;
	char type;
	int id;
}SPLOGIN;

typedef struct ClientPacketTypePlayer
{
	short size;
	char type;
	int id;
	char scene_state;
	PLAYERINFO info;
}SPPLAYER;

typedef struct ClientPacketOtherPlayers
{
	int id;
	char scene_state;
	PLAYERINFO info;
}SPOTHERPLAYERS;

typedef struct ClientPacketMonster
{
	int monster_id;
	MONSTERINFO info;
}SPMONSTER;

typedef struct ClientPacketHit
{
	short size;
	char type;
	int id;
	int monster_id;
	int damage;
}SPHIT;

typedef struct ServerPacketEnd
{
	short size;
	char type;
	int id;
	char scene_state;
}SPEND;

#pragma pack(pop)