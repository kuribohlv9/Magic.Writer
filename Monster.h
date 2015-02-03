#pragma once
#include "GameObject.h"

enum ItemProperties
{
	HARD,
	SOFT,
	ALIVE,
	DEAD
};

class DrawManager;

class Monster : public GameObject
{
public:
	Monster(sf::Texture* texture, float x, float y, float speed, int health, ItemProperties weakness);
	~Monster();

	void Draw(DrawManager* drawManager);
	void Update(float deltaTime);
	void Freeze(float time);
	void Damage(ItemProperties propertyOne, ItemProperties propertyTwo);

private:
	ItemProperties m_weakness;
	bool m_frozen;
	int m_health;
	float m_speed;
	float m_freezeTimer;
	float m_unfreezeDelay;
};