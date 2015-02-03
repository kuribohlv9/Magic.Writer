#include "stdafx.h"
#include "Monster.h"
#include "DrawManager.h"
#include "Collider.h"

// INTE KLAR HÄR ÄN!!!

Monster::Monster(sf::Texture* texture, float x, float y, float speed, int health, ItemProperties weakness)
{
	m_type = ENTITY_MONSTERS;

	m_x = x;
	m_y = y;

	m_weakness = weakness;
	m_speed = speed;
	m_health = health;
	m_frozen = false;

	m_sprite.setTexture(*texture);
	m_collider = new Collider(m_x, m_y);
	m_collider->SetParent(this);
	m_collider->SetWidthHeight(m_sprite.getTextureRect().width, m_sprite.getTextureRect().height);
}

Monster::~Monster()
{
	if (m_collider)
		delete m_collider;
}

void Monster::Draw(DrawManager* drawManager)
{
	m_sprite.setPosition(m_x, m_y);
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
}

void Monster::Update(float deltaTime)
{
	if (!m_frozen)
	{
		m_y += m_speed * deltaTime;
		m_collider->Refresh();
	}
}

void Monster::Freeze()
{
	//Inverts the boolean.
	m_frozen = !m_frozen;
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