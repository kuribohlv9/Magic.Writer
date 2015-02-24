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
	m_animator = new Animator(&m_sprite, "assets/sprites/wizard/wizard_animation.txt");
	m_animator->SetAnimation("idle");

	//Set origin
	sf::IntRect textureRect = m_sprite.getTextureRect();
	m_sprite.setOrigin(textureRect.width / 2.0f, textureRect.height / 2.0f);

	//Set collider
	m_collider = new Collider(-33, 40);
	m_collider->SetParent(this);
	m_collider->SetWidthHeight(textureRect.width / 2, textureRect.height - 100);

	//Set start position
	m_lane = 1;
	ChangeLane(1);
}

Player::~Player()
{
	//Delete the newed animator
	if (m_animator)
	{
		delete m_animator;
	}

	//Point item towards nullptr
	m_item = nullptr;
}

void Player::Update(float deltaTime)
{
	//Handle movement between lanes
	if (!IsStunned())
		HandleMovement();

	switch (m_state)
	{
	case PLAYER_HOLDING:
		break;
	case PLAYER_THROWING:
		m_animator->Update(deltaTime);

		if (m_animator->Complete())
		{
			m_state = PLAYER_IDLE;
			m_animator->SetAnimation("idle");
		}
		break;
	case PLAYER_KNOCKEDDOWN:
		m_animator->Update(deltaTime);
		if (m_animator->Complete())
		{
			m_state = PLAYER_IDLE;
			m_animator->SetAnimation("idle");
			m_sprite.setScale(1, 1);
		}
		break;
	case PLAYER_JUMPING:
		break;
	}	
}

void Player::Draw(DrawManager* drawManager)
{
	//Draw player sprite
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
	
	//Draw possible item
	if (m_item)
	{
		m_item->Draw(drawManager);
	}
}

void Player::ChangeLane(int xDirection)
{
	//Change lane number
	m_lane += xDirection;

	//Keep lane number within bounds (0 and 4)
	if (m_lane < 0)
		m_lane = 0;
	else if (m_lane > 4)
		m_lane = 4;

	//Set player position
	SetPosition(Lanes[m_lane], 800);

	//Set possible item position
	if (m_item)
	{
		m_item->SetPosition(m_x - 25, m_y - 60);
	}
}

void Player::SetItem(Item* item)
{
	//Apply or remove conjured item to player
	m_item = item;

	//If applied
	if (m_item)
	{
		//Change items scale to fit hands
		m_item->GetSprite()->setScale(1, 1);

		//Update items position
		ChangeLane(0);

		//Change state to holding and change animation
		m_state = PLAYER_HOLDING;
		m_animator->SetAnimation("idle");
	}
	else //If removed
	{
		//Change state to throwing and change animation
		m_state = PLAYER_THROWING;
		m_animator->SetAnimation("throwing");
	}
}

Item* Player::GetItem()
{
	//Returns an item if the wizard is holding one, else it returns nullptr
	return m_item;
}

void Player::HandleMovement()
{
	//Change lane with arrowkeys
	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Left))
	{
		ChangeLane(-1);
	}
	else if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Right))
	{
		ChangeLane(1);
	}
}

void Player::Knockdown()
{
	if (m_state == PLAYER_KNOCKEDDOWN)
		return;
	
	m_state = PLAYER_KNOCKEDDOWN;
	m_animator->SetAnimation("knockeddown");
	int knockDirection = rand() % 2;

	if (knockDirection == 0)
		knockDirection = -1;
	else
		knockDirection = 1;

	int newLane = m_lane + knockDirection;

	if (newLane < 0 || newLane > 4)
	{
		knockDirection *= -1;
	}
	m_sprite.setScale(knockDirection, 1);

	ChangeLane(knockDirection);
}

bool Player::IsStunned()
{
	return (m_state == PLAYER_KNOCKEDDOWN);
}

int Player::GetCurrentLane()
{
	return m_lane;
}

void Player::ChantingAnimation()
{
	if (m_state == PLAYER_IDLE)
	{
			m_state = PLAYER_CHANTING;
			m_animator->SetAnimation("chanting");
	}
	if (m_state == PLAYER_CHANTING)
	{
		m_animator->Update(1000);	
	}

}