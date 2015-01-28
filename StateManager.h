#pragma once
#include "State.h"

class StateManager
{
public:
	StateManager();
	~StateManager();

	bool Initialize();
	void Shutdown();
	bool Update(float deltaTime);
	void Draw();
	bool ChangeState(ScreenState nextState);

	void SetActiveState(ScreenState screenState);
	void AddState(ScreenState screenState, State* state);
	void RemoveState(ScreenState screenState);

private:
	State* m_activeState;
	std::map<ScreenState, State*> m_states;
};