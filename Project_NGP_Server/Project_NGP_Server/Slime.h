#pragma once
#include "Monster.h"

class Slime
	: public Monster
{
public:
	Slime();
	virtual ~Slime();

public:
	virtual bool Initialize(int pos_x, int pos_y, int width, int height);
	virtual int  Update(const float& TimeDelta);
	virtual void Release();
};


