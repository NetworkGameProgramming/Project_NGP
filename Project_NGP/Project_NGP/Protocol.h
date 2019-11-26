#pragma once

#define SERVER_PORT 3500

// Ŭ���̾�Ʈ ��Ŷ ó��
#define SP_LOGIN_OK			1		// �α���
#define SP_PLAYER			2		// �÷��̾� ��ġ
#define SP_OTHERPLAYER		3
#define SP_END				4		// ���� ����

// �ִ� ���� ���� ũ��
constexpr int MAX_BUFFER = 1024;

// Overlapped IO�� �������� (p.433)
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