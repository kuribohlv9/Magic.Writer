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
	GUI_Button* m_helpButton;
	GUI_Button* m_helpExitButton;
	sf::Sprite m_poleSprite;

	bool m_helpScreen;
	bool m_showHighscore;

	sf::Sprite m_backgroundSprite;
};