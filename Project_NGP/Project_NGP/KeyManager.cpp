#include "stdafx.h"
#include "KeyManager.h"

IMPLEMENT_SINGLETON(KeyManager)

KeyManager::KeyManager()
{
	ZeroMemory(&m_KeyArr, sizeof(KEY_MAX) * STATE_END);
}

KeyManager::~KeyManager()
{
}

bool KeyManager::GetKeyState(const KEYSTATE& KeyState, const int & VirtualKey)
{
	if (VirtualKey >= KEY_MAX ||
		KeyState >= STATE_END)
	{
		return false;
	}

	return m_KeyArr[KeyState][VirtualKey];
}

int KeyManager::UpdateKey()
{
	ComputeKeyState(VK_LEFT);
	ComputeKeyState(VK_RIGHT);
	ComputeKeyState(VK_UP);
	ComputeKeyState(VK_DOWN);
	
	return 0;
}

void KeyManager::ComputeKeyState(const int & VirtualKey)
{
	bool& KeyStateDown = m_KeyArr[STATE_DOWN][VirtualKey];
	bool& KeyStatePush = m_KeyArr[STATE_PUSH][VirtualKey];
	bool& KeyStateUp = m_KeyArr[STATE_UP][VirtualKey];

	if (0 != GetAsyncKeyState(VirtualKey))
	{
		if (false == KeyStateDown &&
			false == KeyStatePush)
		{
			KeyStateDown = true;
			KeyStatePush = false;
			KeyStateUp = false;
		}
		else
		{
			KeyStateDown = false;
			KeyStatePush = true;
			KeyStateUp = false;
		}
	}
	else
	{
		if (true == KeyStateUp)
		{
			KeyStateDown = false;
			KeyStatePush = false;
			KeyStateUp = false;
		}
		else
		{
			KeyStateDown = false;
			KeyStatePush = false;
			KeyStateUp = true;
		}
	}
}
