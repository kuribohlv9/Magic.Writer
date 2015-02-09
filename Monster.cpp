#include "stdafx.h"
#include "Monster.h"
#include "DrawManager.h"
#include "Collider.h"
#include <time.h>

Monster::Monster(sf::Texture* texture, float speed, int health)
{
	srand(time(NULL));
	//Variables
	m_type = GAMEOBJECT_MONSTERS;
	m_speed = speed;
	m_health = health;
	m_frozen = false;

	//Sprite
	m_sprite.setTexture(*texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, 418, 418));
	m_sprite.setScale(0.6f, 0.6f);
	sf::IntRect textureRect = m_sprite.getTextureRect();
	m_sprite.setOrigin(textureRect.width / 2.0f, textureRect.height / 2.0f);

	//Collider
	m_collider = new Collider(0, 0);
	m_collider->SetParent(this);
	m_collider->SetWidthHeight(textureRect.width * 0.4f, textureRect.height * 0.4f);

	//Position
	int randomX = rand() % 5;
	sf::Vector2f position = sf::Vector2f(192 + 384 * randomX, -textureRect.height);
	SetPosition(position.x, 0);

	//Weakness
	int randomWeakness = rand() % 4;
	ItemProperty weakness;
	switch (randomWeakness)
	{
	case 0:
		weakness = ITEM_ALIVE;
		m_sprite.setColor(sf::Color(50, 50, 50, 255));
		break;
	case 1:
		weakness = ITEM_DEAD;
		break;
	case 2:
		weakness = ITEM_COLD;
		m_sprite.setColor(sf::Color::Red);
		break;
	case 3:
		weakness = ITEM_HOT;
		m_sprite.setColor(sf::Color::Blue);
		break;
	}
	m_weakness = weakness;
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