#pragma once
#include "Monster.h"

class BlueSnail
	: public Monster
{
public:
	BlueSnail();
	virtual ~BlueSnail();

public:
	virtual bool Initialize(int pos_x, int pos_y, int width, int height);
	virtual int  Update(const float& TimeDelta);
	virtual void Release();
};

