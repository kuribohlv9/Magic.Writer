#include "stdafx.h"
#include "Monsters.h"

// INTE KLAR H�R �N!!!

Monsters::Monsters(sf::Texture* texture, int x, int y)
{
	m_type = ENTITY_MONSTERS;

	m_x = x;
	m_y = y;

	m_sprite->setTexture(*texture);
	
}

Monsters::~Monsters()
{

}

void Monsters::Draw(sf::RenderWindow window)
{

}

void Monsters::Update(float deltaTime)
{

}

sf::Sprite* Monsters::GetSprite()
{
	return m_sprite;
}

sf::IntRect* Monsters::GetHitBox()
{
	return m_hitBox;
}

bool Monsters::IsActive()
{
	return m_active;
}

float Monsters::GetX()
{
	return m_x;
}

float Monsters::GetY()
{
	return m_y;
}