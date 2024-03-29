#pragma once
#include "State.h"

class HighscoreManager;
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
	HighscoreManager* m_highscoreManager;
	DrawManager* m_drawManager;
	TextureManager* m_textureManager;
	ScreenState m_nextState;

	//GUI elements
	std::vector<GUI_Button*> m_buttons;
	GUI_Button* m_playButton;
	GUI_Button* m_exitButton;
	GUI_Button* m_highscoreButton;
	sf::Sprite m_poleSprite;
	sf::Sprite m_highscoreSprite;

	sf::Sprite m_backgroundSprite;
};