#pragma once
#include "State.h"

class DrawManager;
class TextureManager;
class GUI_Button;

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
	GUI_Button* m_highscoreButton;
	GUI_Button* m_helpButton;
	sf::Sprite m_poleSprite;

	sf::Sprite m_backgroundSprite;
};