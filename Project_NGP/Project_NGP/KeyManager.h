#pragma once
class KeyManager
{
	DECLARE_SINGLETON(KeyManager);
private:
	explicit KeyManager();
	virtual ~KeyManager();

public:
	bool GetKeyState(const KEYSTATE& KeyState, const int& VirtualKey);

public:
	int UpdateKey();

private:
	void ComputeKeyState(const int& VirtualKey);

private:
	bool m_KeyArr[STATE_END][KEY_MAX];
};

