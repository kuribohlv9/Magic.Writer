#pragma once
#include "State.h"

class DrawManager;

class LosingState : public State
{
public:
	LosingState();
	~LosingState();

	bool Update(float deltaTime);
	void Draw();

	void Enter();
	void Exit();

	ScreenState NextState();

private:
	DrawManager* m_drawManager;
};

