#pragma once
class KeyManager : public Singleton<KeyManager>
{
public:
	KeyManager();
	virtual ~KeyManager();

public:
	const bool GetKeyState(const KEYSTATE& KeyState, const int& VirtualKey);

public:
	int UpdateKey();

private:
	void ComputeKeyState(const int& VirtualKey);

private:
	// 2���� �迭�� ���� Ű ���� ����� Ű�� �迭�� 
	bool m_KeyArr[STATE_END][KEY_MAX];
};

