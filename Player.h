#pragma once
#include "GameObject.h"

class DrawManager;
class InputManager;
class Item;

class Player : public GameObject
{
public:
	Player(sf::Texture* texture);
	void Update(float deltaTime);
	void Draw(DrawManager* drawManager);

	void SetItem(Item* item);
	Item* GetItem();

private:
	void ChangeLane(int xDirection);

private:
	InputManager* m_inputManager;
	Item* m_item;
	int m_lane;
};