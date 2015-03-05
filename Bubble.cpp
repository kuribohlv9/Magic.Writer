#include "stdafx.h"
#include "Bubble.h"
#include "DrawManager.h"
#include "Item.h"
#include "Player.h"
#include <iostream>

Bubble::Bubble(float x, float y, sf::Texture* texture, Player* player)
{
	m_width = texture->getSize().x / 4;
	m_height = texture->getSize().y;

	m_sprite.setPosition(x, y);
	m_sprite.setTexture(*texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_width, m_height));
	m_sprite.setOrigin(m_width / 2, m_height / 2);

	m_item = nullptr;
	m_player = player;
	m_playerOffset = m_sprite.getPosition().x - player->GetX();

	m_lifeTime += 75 + x * 100;


	m_lanePositions[0] = 354;
	m_lanePositions[1] = 684;
	m_lanePositions[2] = 960;
	m_lanePositions[3] = 1256;
	m_lanePositions[4] = 1586;
}

void Bubble::SetAlpha(int alpha)
{
	m_sprite.setColor(sf::Color(255, 255, 255, alpha));
	
	if (m_item)
	{
		m_item->GetSprite()->setColor(sf::Color(255, 255, 255, alpha));
	}
}
void Bubble::SetItem(Item* item)
{
	m_item = item;

	int x = (int)(item->GetProperty());
	m_sprite.setTextureRect(sf::IntRect(x * m_width, 0, m_width, m_height));
}
Item* Bubble::GetItem()
{
	return m_item;
}
sf::Vector2f Bubble::GetPosition()
{
	return m_sprite.getPosition();
}

void Bubble::Update(float deltaTime)
{
	m_lifeTime += deltaTime;

	sf::Vector2f pos = m_sprite.getPosition();
	int targetX = m_lanePositions[m_player->GetCurrentLane()] + m_playerOffset;
	float distance = abs(pos.x - targetX);
	int dir = (pos.x < targetX) ? 1 : -1;

	m_sprite.move(distance * 2 * dir * deltaTime, 0);

	pos.y += 0.25f * cos(m_lifeTime * 3.0f);

	m_sprite.setPosition(m_sprite.getPosition().x, pos.y);

	//Set item position
	if (m_item)
	{
		m_item->SetPosition(pos.x, pos.y - 15);
	}
}
void Bubble::Draw(DrawManager* drawManager)
{
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
	if (m_item)
		m_item->Draw(drawManager);
}