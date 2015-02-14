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
	m_sprite.setTextureRect(sf::IntRect(0, 0, 207, 207));
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
		m_color = sf::Color(100, 100, 100, 255);
		break;
	case 1:
		weakness = ITEM_DEAD;
		break;
	case 2:
		weakness = ITEM_COLD;
		m_color = sf::Color::Red;
		break;
	case 3:
		weakness = ITEM_HOT;
		m_color = sf::Color::Blue;
		break;
	}
	m_weakness = weakness;
}

void Monster::Draw(DrawManager* drawManager)
{
	if (!m_frozen)
	{
		//Change snails y scale
		float y_scale = 1.0f + abs(0.1f * cos(m_totalLifeTime * 3.0f));
		if (y_scale >= 1.3f)
			y_scale = 1.3f;
		m_sprite.setScale(1, y_scale);

		//Draw snail
		m_sprite.setTextureRect(sf::IntRect(207, 0, 207, 207));
		drawManager->Draw(m_sprite, sf::RenderStates::Default);

		//Reset scale for the other parts
		m_sprite.setScale(1, 1);

		//Draw torso part
		m_sprite.setTextureRect(sf::IntRect(207 * 2, 0, 207, 207));
		drawManager->Draw(m_sprite, sf::RenderStates::Default);


		//Draw ocean foam
		m_sprite.setTextureRect(sf::IntRect(207 * 3, 0, 207, 207));
		drawManager->Draw(m_sprite, sf::RenderStates::Default);

		//Draw head
		m_sprite.setTextureRect(sf::IntRect(0, 207, 207, 207));
		drawManager->Draw(m_sprite, sf::RenderStates::Default);
	}
}

void Monster::Update(float deltaTime)
{
	m_totalLifeTime += deltaTime;

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
		m_health -= 2;
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