#pragma once

#define SERVER_PORT 3500

// 클라이언트 패킷 처리
#define SP_LOGIN_OK			1		// 로그인
#define SP_PLAYER			2		// 플레이어 위치
#define SP_OTHERPLAYER		3
#define SP_END				4		// 접속 종료

// 최대 보낼 버퍼 크기
constexpr int MAX_BUFFER = 1024;

// Overlapped IO용 소켓정보 (p.433)
typedef struct OverInfo
{
	WSAOVERLAPPED	overlapped;
	WSABUF			wsabuf;
	DWORD			recvbytes;
	DWORD			sendbytes;
	char			buffer[MAX_BUFFER];
}OVERLAPPEDINFO;

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
	short pos_x, pos_y;
	int player_state;
}SPPLAYER;

typedef struct ClientPacketOtherPlayers
{
	int id;
	short pos_x, pos_y;
	int player_state;
}SPOTHERPLAYERS;

typedef struct ServerPacketEnd
{
	char size;
	char type;
	int id;
}SPEND;

#pragma pack(pop)