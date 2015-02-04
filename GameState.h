#pragma once
#include "State.h"

class ItemManager;
class WordManager;
class DrawManager;
class TextureManager;
class InputManager;

class Item;
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
	void ConvertWordToItem();

private:
	DrawManager* m_drawManager;
	TextureManager* m_textureManager;
	InputManager* m_inputManager;
	WordManager* m_wordManager;
	ItemManager* m_itemManager;

	Item* m_spawnedItem;
	Item* m_items[3];
	std::vector<Item*> m_activeItems;
	int m_itemCount;
};