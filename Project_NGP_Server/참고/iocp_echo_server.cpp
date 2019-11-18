#include <iostream>
#include <map>
#include <thread>
using namespace std;
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include "protocol.h"

#define MAX_BUFFER        1024

struct OVER_EX {
	WSAOVERLAPPED over;
	WSABUF	wsabuf[1];
	char	net_buf[MAX_BUFFER];
	bool	is_recv;
};

struct SOCKETINFO
{
	OVER_EX	recv_over;
	SOCKET	socket;
	int		id;
	// game contents
	short	 x, y;
};

map <int, SOCKETINFO> clients;
HANDLE	g_iocp;
int new_user_id;

void error_display(const char *msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	cout << msg;
	wcout << L"에러 " << lpMsgBuf << endl;
	while (true);
	LocalFree(lpMsgBuf);
}

void send_packet(int id, void* buff)
{
	char* packet = reinterpret_cast<char*>(buff);
	int packet_size = packet[0];


	OVER_EX* send_over = new OVER_EX;
	memset(send_over, 0x00, sizeof(OVER_EX));
	send_over->is_recv = false; // recv 아니다~
	memcpy(send_over->net_buf, packet, packet_size);
	send_over->wsabuf[0].buf = send_over->net_buf;
	send_over->wsabuf[0].len = packet_size;
	WSASend(clients[id].socket, send_over->wsabuf, 1, 0, 0, &send_over->over, 0);
}

void sned_login_ok_packet(int id)
{
	sc_packet_login_ok packet;
	packet.id = id;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_OK;
	send_packet(id, &packet);

}

void send_put_player_packet(int client, int new_id)
{
	sc_packet_put_player packet;
	packet.id = new_id;
	packet.size = sizeof(packet);
	packet.type = SC_PUT_PLAYER;
	packet.x = clients[new_id].x;
	packet.y = clients[new_id].y;
	send_packet(client, &packet);

}

void send_pos_packet(int client, int mover)
{// id의 위치정보 보내기
	sc_packet_pos packet;
	packet.id = mover;
	packet.size = sizeof(packet);
	packet.type = SC_POS;
	packet.x = clients[mover].x;
	packet.y = clients[mover].y;
	send_packet(client, &packet);

}

void ProcessPacket(int id, void* buff)
{
	char* packet = reinterpret_cast<char*> (buff); // 내용 읽기위해 타입 캐스팅
	short x = clients[id].x;
	short y = clients[id].y;

	switch (packet[1]) {
	case CS_UP:
		if (y > 0) --y;
		break;
	case CS_DOWN:
		if (y < WORLD_HEIGHT - 1) ++y;
		break;
	case CS_LEFT:
		if (x > 0) --x;
		break;
	case CS_RIGHT:
		if (x < WORLD_WIDTH - 1) ++x;
		break;
	default: cout << "Invalid Packet Type Error\n";
		while (1);
	}

	// Update Position
	clients[id].x = x;
	clients[id].y = y;

	for (auto& cl : clients) {
		send_pos_packet(cl.first, id); // 모든 클라에게 id의 변경된 위치 보내기
	}
}

void do_worker()
{
	while (true) {
		DWORD num_byte;
		ULONG key;
		PULONG p_key = &key;
		WSAOVERLAPPED *p_over;

		GetQueuedCompletionStatus(g_iocp, &num_byte, (PULONG_PTR)p_key, &p_over, INFINITE);

		SOCKET client_s = clients[key].socket;

		if (num_byte == 0) {
			closesocket(client_s);
			clients.erase(client_s);
			continue;
		}  // 클라이언트가 closesocket을 했을 경우

		OVER_EX *over_ex = reinterpret_cast<OVER_EX *> (p_over);

		if (true == over_ex->is_recv) {
			ProcessPacket(key, over_ex->net_buf); // key가 보낸 패킷을 처리해라.

			DWORD flags = 0;
			memset(&over_ex->over, 0x00, sizeof(WSAOVERLAPPED)); // 0으로 초기화 후 재사용.
			WSARecv(client_s, over_ex->wsabuf, 1, 0, &flags, &over_ex->over, 0);
		}
		else {
			delete over_ex;
		}
	}
}

int main()
{
	wcout.imbue(std::locale("korean"));
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN));
	listen(listenSocket, 5);
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	memset(&clientAddr, 0, addrLen);
	SOCKET clientSocket;
	DWORD flags;

	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	thread worker_thread{ do_worker };
	thread worker_thread2{ do_worker };
	thread worker_thread3{ do_worker };
	while (true) {
		clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen);
		int user_id = new_user_id++;

		clients[user_id] = SOCKETINFO{};
		memset(&clients[clientSocket], 0, sizeof(struct SOCKETINFO));
		clients[user_id].socket = clientSocket;
		clients[user_id].recv_over.wsabuf[0].len = MAX_BUFFER;
		clients[user_id].recv_over.wsabuf[0].buf = clients[user_id].recv_over.net_buf;
		clients[user_id].recv_over.is_recv = true;
		flags = 0;
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), g_iocp, user_id, 0);

		sned_login_ok_packet(user_id);
		clients[user_id].x = 4;
		clients[user_id].y = 4; // initialize pos

		for (auto& cl : clients) {
			int other_player = cl.first;

			send_put_player_packet(other_player, user_id);
			if (other_player != user_id) // 두번 보내면 안됨.
				send_put_player_packet(user_id, other_player);

		}


		int ret = WSARecv(clientSocket, clients[user_id].recv_over.wsabuf, 1, NULL,
			&flags, &(clients[user_id].recv_over.over), NULL);
		if (0 != ret) {
			int err_no = WSAGetLastError();
			if (WSA_IO_PENDING != err_no)
				error_display("WSARecv Error :", err_no);
		}
	}
	worker_thread.join();
	closesocket(listenSocket);
	WSACleanup();
}

