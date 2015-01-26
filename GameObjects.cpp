//GameObjects.cpp

#include "stdafx.h"
#include "GameObjects.h"

GameObjects::GameObjects()
{
	// Default values.
	m_x = 0;
	m_y = 0;
	m_active = true;
	m_sprite = nullptr;
	m_hitBox = nullptr;
	
}

GameObjects::~GameObjects()
{
	if (m_hitBox)
		delete m_hitBox;
}

sf::Sprite* GameObjects::GetSprite()
{
	return m_sprite;
}

sf::IntRect* GameObjects::GetHitBox()
{
	return m_hitBox;
}

bool GameObjects::IsActive()
{
	return m_active;
}

float GameObjects::GetX()
{
	return m_x;
}

float GameObjects::GetY()
{
	return m_y;
}