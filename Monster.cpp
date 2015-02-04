#include "stdafx.h"
#include "Monster.h"
#include "DrawManager.h"
#include "Collider.h"

Monster::Monster(sf::Texture* texture, float x, float y, float speed, int health, ItemProperties weakness)
{
	m_type = GAMEOBJECT_MONSTERS;

	m_weakness = weakness;
	m_speed = speed;
	m_health = health;
	m_frozen = false;

	m_sprite.setTexture(*texture);
	m_collider = new Collider(x, y);
	m_collider->SetParent(this);
	m_collider->SetWidthHeight(m_sprite.getTextureRect().width, m_sprite.getTextureRect().height);

	SetPosition(x, y);
}

void Monster::Draw(DrawManager* drawManager)
{
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
}

void Monster::Update(float deltaTime)
{
	if (!m_frozen)
	{
		Move(0, m_speed * deltaTime);
	}
	else
	{
		m_freezeTimer += deltaTime;

		if (m_freezeTimer >= m_unfreezeDelay)
		{
			m_freezeTimer = 0;
			m_unfreezeDelay = 0;
			m_frozen = false;
		}
	}
}

void Monster::Freeze(float time)
{
	m_unfreezeDelay = time;
	m_frozen = true;
}

void Monster::Damage(ItemProperties propertyOne, ItemProperties propertyTwo)
{
	if (propertyOne == m_weakness || propertyTwo == m_weakness)
	{
		m_health -= 3;
	}
	else
	{
		m_health -= 1;
	}
}