#include "stdafx.h"
#include "Player.h"
#include "Collider.h"
#include "Item.h"

#include "InputManager.h"
#include "DrawManager.h"
#include "ServiceLocator.h"

Player::Player(sf::Texture* texture)
{
	m_inputManager = ServiceLocator<InputManager>::GetService();
	m_item = nullptr;

	m_type = GAMEOBJECT_PLAYER;

	m_sprite.setTexture(*texture);
	sf::IntRect textureRect = m_sprite.getTextureRect();

	m_collider = new Collider(m_x, m_y);
	m_collider->SetParent(this);
	m_collider->SetWidthHeight(textureRect.width, textureRect.height);
	
	m_sprite.setOrigin(textureRect.width / 2.0f, textureRect.height / 2.0f);

	m_lane = 1;
	ChangeLane(1);
}
void Player::Update(float deltaTime)
{
	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Left))
	{
		ChangeLane(-1);
	}
	else if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Right))
	{
		ChangeLane(1);
	}
}
void Player::Draw(DrawManager* drawManager)
{
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
	
	if (m_item)
	{
		m_item->Draw(drawManager);
	}
}

void Player::ChangeLane(int xDirection)
{
	m_lane += xDirection;

	if (m_lane < 0)
		m_lane = 0;
	else if (m_lane > 4)
		m_lane = 4;

	SetPosition(192.0f + 384.0f * m_lane, 750.0f);

	if (m_item)
	{
		m_item->SetPosition(m_x, m_y - m_item->GetSprite()->getTextureRect().height);
	}
}

void Player::SetItem(Item* item)
{
	m_item = item;

	if (m_item)
	{
		m_item->SetPosition(m_x, m_y - m_item->GetSprite()->getTextureRect().height);
	}
}
Item* Player::GetItem()
{
	return m_item;
}