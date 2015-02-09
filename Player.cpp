#include "stdafx.h"
#include "Player.h"
#include "Collider.h"
#include "Item.h"
#include "Animator.h"

#include "InputManager.h"
#include "DrawManager.h"
#include "ServiceLocator.h"

Player::Player(sf::Texture* texture)
{
	//Variables
	m_inputManager = ServiceLocator<InputManager>::GetService();
	m_item = nullptr;
	m_type = GAMEOBJECT_PLAYER;
	m_state = PLAYER_IDLE;

	//Animator
	m_sprite.setTexture(*texture);
	m_animation = new Animator(&m_sprite, "assets/sprites/wizard/wizard_animation.txt");
	m_animation->SetAnimation("idle");

	//Set origin
	sf::IntRect textureRect = m_sprite.getTextureRect();
	m_sprite.setOrigin(textureRect.width / 2.0f, textureRect.height / 2.0f);

	//Set collider
	m_collider = new Collider(m_x, m_y);
	m_collider->SetParent(this);
	m_collider->SetWidthHeight(textureRect.width, textureRect.height);

	//Set start position
	m_lane = 1;
	ChangeLane(1);
}
void Player::Update(float deltaTime)
{
	HandleMovement();

	switch (m_state)
	{
	case PLAYER_IDLE:
		//Check for keyboard input, if true.... switch to chanting state
		if (m_inputManager->GetInputChar() != ' ')
		{
			m_state = PLAYER_CHANTING;
			m_animation->SetAnimation("chanting");
		}
		break;

	case PLAYER_CHANTING:
		if (m_inputManager->GetInputChar() != ' ')
		{
			m_animation->Update(1000);
		}
		break;

	case PLAYER_HOLDING:
		break;
	case PLAYER_THROWING:
		m_animation->Update(deltaTime);

		if (m_animation->Complete())
		{
			m_state = PLAYER_IDLE;
			m_animation->SetAnimation("idle");
		}
		break;
	case PLAYER_KNOCKEDDOWN:
		m_animation->Update(deltaTime);
		if (m_animation->Complete())
		{
			m_state = PLAYER_IDLE;
		}
		break;
	case PLAYER_JUMPING:
		break;
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

	SetPosition(192.0f + 384.0f * m_lane, 700);

	if (m_item)
	{
		m_item->SetPosition(m_x - 50, m_y - m_item->GetSprite()->getTextureRect().height + 100);
	}
}

void Player::SetItem(Item* item)
{
	m_item = item;

	if (m_item)
	{
		m_item->SetPosition(m_x - 50, m_y - m_item->GetSprite()->getTextureRect().height + 100);
		m_state = PLAYER_HOLDING;
		m_animation->SetAnimation("idle");
	}
	else
	{
		m_state = PLAYER_THROWING;
		m_animation->SetAnimation("throwing");
	}
}
Item* Player::GetItem()
{
	return m_item;
}

void Player::HandleMovement()
{
	//Change lane
	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Left))
	{
		ChangeLane(-1);
	}
	else if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Right))
	{
		ChangeLane(1);
	}
}