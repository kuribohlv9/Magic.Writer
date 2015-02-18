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
	m_speed = 30;

	m_sprite.setPosition(x, y);
	m_sprite.setTexture(*texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_width, m_height));
	m_sprite.setOrigin(m_width / 2, m_height / 2);

	m_item = nullptr;
	m_player = player;
	m_playerOffset = m_sprite.getPosition().x - player->GetX();
}


void Bubble::SetItem(Item* item)
{
	m_item = item;
	m_item->SetPosition(m_sprite.getPosition().x, m_sprite.getPosition().y - 10);
	
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
	float playerX = m_player->GetX() + m_playerOffset;
	float bubbleX = m_sprite.getPosition().x;
	float direction = m_speed;

	float max = playerX;
	float min = bubbleX;

	if (max < min)
	{
		int tmp = max;
		max = min;
		min = tmp;
		direction *= -1;
	}

	float length = (max - min) / max;

	direction *= length;
	

	m_sprite.move(direction, 0);

	sf::Vector2f pos = m_sprite.getPosition();
	if (pos.x <= m_width / 2)
	{
		m_sprite.setPosition(m_width / 2, pos.y);
	}
	else if (pos.x > ScreenWidth - m_width / 2)
	{
		m_sprite.setPosition(ScreenWidth - m_width / 2, pos.y);
	}
	if (m_item)
	{
		m_item->SetPosition(m_sprite.getPosition().x, m_sprite.getPosition().y);
	}
}
void Bubble::Draw(DrawManager* drawManager)
{
	int x = (int)(m_item->GetProperty());

	
	m_sprite.setTextureRect(sf::IntRect(x * m_width, 0, m_width, m_height));
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
	if (m_item)
		m_item->Draw(drawManager);

	m_sprite.setTextureRect(sf::IntRect(0, m_height, m_width, m_height));
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
}