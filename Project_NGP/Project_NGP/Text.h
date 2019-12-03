#pragma once

class Text
{
public:
	Text();
	virtual ~Text();

public:
	void SetPosition(int x, int y);
	void SetText(const TCHAR* buffer);
	void SetNewlineCount(int count);
	void SetBackColor(unsigned char r, unsigned char g, unsigned char b);
	void SetBackMode(int mode);
	void SetAlign(int align);
public:
	bool Initialize(int size);
	void Render(HDC hdc);
	void Release();

private:
	HFONT m_OldFont;
	HFONT m_hFont;
	TCHAR m_TextBuffer[64] = { 0, };
	POSITION m_StartPos;
	
	int m_NewlineCount = 10;
	int m_Size = 0;

	PIXEL24 m_BackColor;
	int m_BackMode = TRANSPARENT;
	int m_Align = TA_LEFT;
};
