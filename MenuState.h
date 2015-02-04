#pragma once
#include "State.h"

class DrawManager;
class TextureManager;
class GUI_Button;
class GUI_Panel;

class MenuState : public State
{
public:
	MenuState();
	~MenuState();

	bool Update(float deltaTime);
	void Draw();

	void Enter();
	void Exit();

	ScreenState NextState();

private:
	DrawManager* m_drawManager;
	TextureManager* m_textureManager;
	ScreenState m_nextState;

	//GUI elements
	GUI_Button* m_playButton;
	GUI_Button* m_exitButton;
	GUI_Panel* m_panel;
};