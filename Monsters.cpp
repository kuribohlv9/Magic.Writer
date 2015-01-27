#include "stdafx.h"
#include "Monsters.h"

// INTE KLAR HÄR ÄN!!!

Monsters::Monsters(sf::Texture*, int x, int y)
{
	m_type = ENTITY_MONSTERS;

	m_x = x;
	m_y = y;
	
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