#pragma once


enum ScreenState
{
	STATE_INVALID,
	STATE_MENU,
	STATE_GAME,
	STATE_LOSING,
	STATE_TEST,
	STATE_INSTRUCTION
};

class State
{
public:
	virtual ~State() {}

	virtual bool Update(float deltaTime) = 0;
	virtual void Draw() = 0;

	virtual void Enter() = 0;
	virtual void Exit() = 0;

	virtual ScreenState NextState() = 0;
};