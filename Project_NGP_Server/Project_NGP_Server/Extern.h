#pragma once
#include <map>
#include "struct.h"

std::map<int, SOCKET_INFO>  g_mapClient;	// ���� ����ü�� ���� STL map �����̳�
HANDLE						g_iocp;			// iocp �ڵ�