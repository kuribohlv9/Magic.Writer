#include "stdafx.h"
#include "StateManager.h"

//States
#include "GameState.h"
#include "MenuState.h"
#include "LosingState.h"
#include "TestState.h"
#include "InstructionState.h"


StateManager::StateManager()
{
	m_activeState = nullptr;
}
StateManager::~StateManager()
{
	auto it = m_states.begin();

	while (it != m_states.end())
	{
		delete it->second;
		++it;
	}
}

bool StateManager::Initialize()
{
	AddState(STATE_GAME, new GameState());
	AddState(STATE_MENU, new MenuState());
	AddState(STATE_LOSING, new LosingState());
	AddState(STATE_TEST, new TestState());
	AddState(STATE_INSTRUCTION, new InstructionState());

	SetActiveState(STATE_MENU);
	return true;
}
void StateManager::Shutdown()
{
	//Delete all states and clear std::map
	auto it = m_states.begin();
	while (it != m_states.end())
	{
		delete (it->second);
		++it;
	}
	m_states.clear();

	m_activeState = nullptr;
}

bool StateManager::Update(float deltaTime)
{
	if (!m_activeState)
		return false;

	if (!m_activeState->Update(deltaTime))
	{
		//If the activeState did not update correctly, change state
		if (!ChangeState(m_activeState->NextState()))
		{
			//Exit StateManager if the next state was invalid or not found
			return false;
		}
	}

	//Successfull state update
	return true;
}
void StateManager::Draw()
{
	if (!m_activeState)
		return;

	//Draw active state
	m_activeState->Draw();
}
bool StateManager::ChangeState(ScreenState screenState)
{
	//Return false if the next state is invalid
	if (screenState == STATE_INVALID)
	{
		return false;
	}

	//Exit the activeState
	m_activeState->Exit();
	m_activeState = nullptr;

	//Set activeState to nextState.
	m_activeState = m_states[screenState];

	//If the state was not found in std::map, return false
	if (m_activeState == nullptr)
	{
		return false;
	}

	//If state was found Enter the new state
	m_activeState->Enter();

	//return true for a succesfull state change
	return true;
}

void StateManager::SetActiveState(ScreenState screenState)
{
	//Change activestate to a new state
	if (m_activeState)
		m_activeState->Exit();
	m_activeState = m_states[screenState];
	m_activeState->Enter();
}
void StateManager::AddState(ScreenState screenState, State* state)
{
	//Add a new state to the std::map
	m_states.insert(std::pair<ScreenState, State*>(screenState, state));
}
void StateManager::RemoveState(ScreenState screenState)
{
	//Deletes and erases the screenState
	delete m_states[screenState];
	m_states[screenState] = nullptr;

	m_states.erase(screenState);
}