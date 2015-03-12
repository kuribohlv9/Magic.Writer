#pragma once

#include "State.h"

class DrawManager;
class TextureManager;
class GUI_Button;

class InstructionState : public State
{
public:
	InstructionState();
	~InstructionState();

	bool Update(float deltaTime);
	void Draw();

	void Enter();
	void Exit();

	ScreenState NextState();

private:
	ScreenState m_nextState;
	TextureManager* m_textureManager;
	DrawManager* m_drawManager;

	// GUI elements
	GUI_Button* m_nextPageButton;
	GUI_Button* m_lastPageButton;
	GUI_Button* m_exitToMenuButton;
	std::vector<GUI_Button*> m_buttons;

	sf::Sprite m_firstHelpPage;
	sf::Sprite m_secondHelpPage;

	bool m_firstPage;

	int satan;
};