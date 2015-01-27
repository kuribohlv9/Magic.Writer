#pragma once

#include "GameObjects.h"

class Monsters : public GameObjects
{
public:
	Monsters(sf::Texture*, int x, int y);
	~Monsters();

	void Draw(sf::RenderWindow window);
	void Update(float deltaTime);

	
	sf::Sprite* GetSprite();
	sf::IntRect* GetHitBox();

	bool IsActive();
	float GetX();
	float GetY();

	

private:
	float m_x;
	float m_y;
	bool m_active;

	sf::Sprite* m_sprite;	
	sf::IntRect* m_hitBox;

};