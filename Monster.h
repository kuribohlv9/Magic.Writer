#pragma once
#include "GameObject.h"

class DrawManager;

class Monster : public GameObject
{
public:
	Monster(sf::Texture* texture, float speed, int health);

	void Draw(DrawManager* drawManager);
	void Update(float deltaTime);
	void Freeze(float time);
	void Damage(ItemProperty property, int &score);

private:
	ItemProperty m_weakness;
	sf::Color m_color;
	float m_totalLifeTime;
	bool m_frozen;
	int m_health;
	float m_speed;
	float m_freezeTimer;
	float m_unfreezeDelay;
};