#include "stdafx.h"
#include "Monster.h"
#include "DrawManager.h"
#include "Collider.h"

Monster::Monster(sf::Texture* texture, float x, float y, float speed, int health, ItemProperty weakness)
{
	m_type = GAMEOBJECT_MONSTERS;

	m_weakness = weakness;
	m_speed = speed;
	m_health = health;
	m_frozen = false;

	m_sprite.setTexture(*texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, 418, 418));
	m_sprite.setScale(0.6f, 0.6f);
	sf::IntRect textureRect = m_sprite.getTextureRect();
	m_sprite.setOrigin(textureRect.width / 2.0f, textureRect.height / 2.0f);

	m_collider = new Collider(x, y);
	m_collider->SetParent(this);
	m_collider->SetWidthHeight(textureRect.width * 0.4f, textureRect.height * 0.4f);

	SetPosition(x, y);
}

void Monster::Draw(DrawManager* drawManager)
{
	if (!m_frozen)
	{
		m_sprite.setTextureRect(sf::IntRect(418, 0, 418, 418)); //Snail
		drawManager->Draw(m_sprite, sf::RenderStates::Default);

		m_sprite.setTextureRect(sf::IntRect(418 * 2, 0, 418, 418)); //Middle
		drawManager->Draw(m_sprite, sf::RenderStates::Default);

		m_sprite.setTextureRect(sf::IntRect(418 * 3, 0, 418, 418)); //Foam
		drawManager->Draw(m_sprite, sf::RenderStates::Default);

		m_sprite.setTextureRect(sf::IntRect(0, 418, 418, 418)); //Head
		drawManager->Draw(m_sprite, sf::RenderStates::Default);
	}
	//drawManager->Draw(m_sprite, sf::RenderStates::Default);
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

void Monster::Damage(ItemProperty property)
{
	if (property == m_weakness)
	{
		m_health -= 3;
	}
	else
	{
		m_health -= 1;
	}

	if (m_health <= 0)
	{
		SetActive(false);
	}
}