#include "stdafx.h"
#include "SmallBubble.h"
#include "DrawManager.h"
#include "Bubble.h"
#include "Player.h"

SmallBubble::SmallBubble(sf::Texture* texture, Player* player, Bubble* targetBubble, float percentageFromPlayer)
{
	m_sprite.setTexture(*texture);
	m_sprite.setOrigin(texture->getSize().x / 2.0f, texture->getSize().y / 2.0f);

	m_acceleration = 0.01f;
	m_velocity = sf::Vector2f(0, -100);
	m_position = sf::Vector2f(player->GetX(), player->GetY());
	m_player = player;
	m_targetBubble = targetBubble;
	m_active = true;
	m_sprite.scale(percentageFromPlayer, percentageFromPlayer);
	m_percantageFromPlayer = percentageFromPlayer;
}

SmallBubble::~SmallBubble()
{
}

void SmallBubble::Update(float deltaTime)
{
	/*sf::Vector2f m_direction = m_targetBubble->GetPosition() - m_position;
	m_velocity += m_acceleration * m_direction;
	m_position += m_velocity * deltaTime;*/

	sf::Vector2f playerPos = sf::Vector2f(m_player->GetX(), m_player->GetY());
	sf::Vector2f diff = m_targetBubble->GetPosition() - playerPos;

	m_position = playerPos + (diff * m_percantageFromPlayer);

	m_sprite.setPosition(m_position);

	/*if (m_sprite.getScale().x < 1)
		m_sprite.scale(1.01f, 1.01f);*/

	/*int length = sqrt(m_direction.x * m_direction.x + m_direction.y * m_direction.y);
	if (length < 50)
	{
		m_active = false;
	}*/
}
void SmallBubble::Draw(DrawManager* drawManager)
{
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
}
bool SmallBubble::IsActive()
{
	return m_active;
}
