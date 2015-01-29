#pragma once
#include "GameObject.h"

class DrawManager;

class Monster : public GameObject
{
public:
	Monster(sf::Texture* texture, int x, int y);
	~Monster();

	void Draw(DrawManager* drawManager);
	void Update(float deltaTime);
};