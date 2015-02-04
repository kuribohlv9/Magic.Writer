#pragma once
#include "State.h"

class ItemManager;
class WordManager;
class DrawManager;
class TextureManager;
class InputManager;

class Item;
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
	Item* m_items[3];
	std::vector<Item*> m_activeItems;
	int m_itemCount;

	//Background
	sf::Sprite m_backgroundSprite;

	//Player
	Player* m_player;
};