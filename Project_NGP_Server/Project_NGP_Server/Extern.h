#pragma once
#include <map>
#include "struct.h"

extern std::map<int, SOCKET_INFO*>  g_mapClient;	// ���� ����ü�� ���� STL map �����̳�
extern HANDLE					    g_iocp;			// iocp �ڵ�
extern int						    g_id;