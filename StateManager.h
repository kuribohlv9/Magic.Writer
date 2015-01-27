#pragma once
#include "State.h"

class StateManager
{
public:
	StateManager();
	~StateManager();

	bool Update(float deltaTime);
	void Draw();
	bool ChangeState(ScreenState nextState);

	void SetActiveState(ScreenState screen);
	void AddState(ScreenState screen, State* state);

private:
	State* m_activeState;
	std::map<ScreenState, State*> m_states;
};