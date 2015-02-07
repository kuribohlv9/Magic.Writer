#pragma once
#include "State.h"

class ItemManager;
class WordManager;
class DrawManager;
class TextureManager;
class InputManager;

class Item;
class Bubble;
class Monster;
class Player;

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
	void ConvertWordToItem();

private:
	//Manager
	DrawManager* m_drawManager;
	TextureManager* m_textureManager;
	InputManager* m_inputManager;
	WordManager* m_wordManager;
	ItemManager* m_itemManager;

	//Item & word variables
	std::vector<Bubble*> m_bubbles;
	std::vector<Item*> m_activeItems;

	//Monster
	std::vector<Monster*> m_monsters;

	//Background
	sf::Sprite m_backgroundSprite;

	//Player
	Player* m_player;
};