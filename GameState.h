#pragma once
#include "State.h"

class WordManager;
class DrawManager;
class TextureManager;
class InputManager;
class Monster;

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
	InputManager* m_inputManager;
	WordManager* m_wordManager;
};