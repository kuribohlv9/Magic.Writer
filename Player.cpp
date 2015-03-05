#include "stdafx.h"
#include "Player.h"
#include "Collider.h"
#include "Item.h"
#include "Animator.h"

#include "InputManager.h"
#include "DrawManager.h"
#include "ServiceLocator.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"

Player::Player(sf::Texture* texture, sf::Texture* particle, sf::SoundBuffer* changeLaneBuffer)
{
	//Variables
	m_inputManager = ServiceLocator<InputManager>::GetService();
	m_item = nullptr;
	m_type = GAMEOBJECT_PLAYER;

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
	m_state = PLAYER_IDLE;
	m_lane = 1;
	ChangeLane(1);
	SetPosition(Lanes[2], 780);
	m_targetX = Lanes[2];
	m_animator->SetAnimation("idle");

	//Set sound
	m_changeLaneSound.setBuffer(*changeLaneBuffer);
	m_changeLaneSound.setVolume(10);

	//Set emitter
	m_emitter = ServiceLocator<ParticleManager>::GetService()->CreateEmitter(particle, 200);
	m_emitter->SetSpawnRate(0);
	m_emitter->SetLifeTime(2, 2);
	m_emitter->SetSize(60, 1);
	m_emitter->SetAcceleration(0, 2);
}
Player::~Player()
{
	//Delete the newed animator
	if (m_animator)
	{
		delete m_animator;
		m_animator = nullptr;
	}

	//Point item towards nullptr
	m_item = nullptr;
}

void Player::Update(float deltaTime)
{
	if (rand() % 2 == 0)
		m_emitter->SetStartAngle(300, 320);
	else
		m_emitter->SetStartAngle(220, 240);

	m_emitter->SetForce(50, 50);
	m_emitter->SetPosition(GetX() + 10, GetY() - 120);

	//Handle movement between lanes
	if (!IsStunned())
		HandleMovement();

	if (m_state != PLAYER_CHANTING)
		m_animator->Update(deltaTime);

	switch (m_state)
	{
	case PLAYER_THROWING:
		if (m_animator->Complete())
		{
			m_state = PLAYER_IDLE;
			m_animator->SetAnimation("idle");
		}
		break;
	case PLAYER_KNOCKEDDOWN:
		if (m_animator->Complete())
		{
			if (m_item)
			{
				m_state = PLAYER_HOLDING;
				m_animator->SetAnimation("holding");
			}
			else
			{
				m_state = PLAYER_IDLE;
				m_animator->SetAnimation("idle");
			}
			m_sprite.setScale(1, 1);
		}
		break;
	case PLAYER_JUMPING:
		if (m_animator->Complete())
		{
			if (m_item)
			{
				m_state = PLAYER_HOLDING;
				m_animator->SetAnimation("holding");
			}
			else
			{
				m_state = PLAYER_IDLE;
				m_animator->SetAnimation("idle");
			}
			m_sprite.setScale(1, 1);
		}
		break;
	}


	//Move player
	float distance = abs(GetX() - m_targetX);
	int dir = (GetX() < m_targetX) ? 1 : -1;

	Move(distance * deltaTime * dir * 7, 0);
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
	{
		m_lane = 0;
		return;
	}
	else if (m_lane > 4)
	{
		m_lane = 4;
		return;
	}

	//Set target position
	m_targetX = Lanes[m_lane];

	//Set possible item position
	if (m_item)
	{
		m_item->SetPosition(m_x - 25, m_y - 170);
	}

	if (xDirection != 0 && m_state != PLAYER_KNOCKEDDOWN)
	{
		//Set animation
		m_state = PLAYER_JUMPING;
		m_animator->SetAnimation("cll");
		m_sprite.setScale(xDirection * -1, 1);
		
		//Play sound
		m_changeLaneSound.stop();
		m_changeLaneSound.setPlayingOffset(sf::seconds(0.4f));
		m_changeLaneSound.play();
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
		m_animator->SetAnimation("holding");
	}
	else //If removed
	{
		//Change state to throwing and change animation
		m_state = PLAYER_THROWING;
		m_animator->SetAnimation("throwing");
	}

	m_emitter->SetActive((item != nullptr));
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
	m_state = PLAYER_KNOCKEDDOWN;
	m_animator->SetAnimation("knockback");
	m_sprite.setScale(knockDirection * 0.9f, 0.9f);

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