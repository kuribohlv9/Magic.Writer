#pragma once
#include "GameObject.h"

class DrawManager;

class Monster : public GameObject
{
public:
	Monster(sf::Texture* texture, float x, float y, float speed, int health, ItemProperty weakness);

	void Draw(DrawManager* drawManager);
	void Update(float deltaTime);
	void Freeze(float time);
	void Damage(ItemProperty property);

private:
	ItemProperty m_weakness;
	bool m_frozen;
	int m_health;
	float m_speed;
	float m_freezeTimer;
	float m_unfreezeDelay;
};