#pragma once
#include "State.h"

class DrawManager;
class TextureManager;

class GameState : public State
{
public:
	GameState();
	~GameState();

	bool Update(float deltaTime);
	void Draw();

	void Enter();
	void Exit();

	ScreenState NextState();

private:
	DrawManager* m_drawManager;
	TextureManager* m_textureManager;

	//TMP
	sf::Sprite m_sprite;
};