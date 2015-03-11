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

Player::Player(sf::Texture* texture, sf::Texture* particle, sf::Texture* sandParticle, sf::SoundBuffer* changeLaneBuffer)
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
	m_targetX = static_cast<float>(Lanes[2]);
	m_animator->SetAnimation("idle");

	//Set sound
	m_changeLaneSound.setBuffer(*changeLaneBuffer);
	m_changeLaneSound.setVolume(10);

	//Set emitter
	m_emitter = ServiceLocator<ParticleManager>::GetService()->CreateEmitter(particle, 100);
	m_emitter->SetActive(false);
	m_emitter->SetSpawnRate(0.2f);
	m_emitter->SetLifeTime(2, 2);
	m_emitter->SetSize(40, 40);
	m_emitter->SetAcceleration(0, 0);
	m_emitter->SetForce(50, 50);

	//Set sand emitter
	m_sandEmitter = ServiceLocator<ParticleManager>::GetService()->CreateEmitter(sandParticle, 200);
	m_sandEmitter->SetActive(false);
	m_sandEmitter->SetSpawnRate(0);
	m_sandEmitter->SetLifeTime(.25f, .25f);
	m_sandEmitter->SetSize(1, 60);
	m_sandEmitter->SetForce(0, 0);
	m_sandEmitter->SetRotationVelocity(2);
	m_sandEmitter->SetAcceleration(0, -2);
	m_sandEmitter->SetScaling(false);
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
	m_sandEmitter->SetPosition(GetX(), GetY() + 70);

	m_emitter->SetPosition(GetX() + 10, GetY() - 150);
	m_emitter->SetStartAngle(0, 360);

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

			m_sandEmitter->SetActive(false);
		}
		break;
	}


	//Move player
	float distance = abs(GetX() - m_targetX);
	int dir = (GetX() < m_targetX) ? 1 : -1;

	Move(distance * deltaTime * dir * 7, 0);

	//Set possible item position
	if (m_item)
	{
		m_item->SetPosition(m_x - 25, m_y - 170);
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
	m_targetX = static_cast<float>(Lanes[m_lane]);

	if (xDirection != 0 && m_state != PLAYER_KNOCKEDDOWN)
	{
		//Set animation
		m_state = PLAYER_JUMPING;
		m_animator->SetAnimation("cll");
		m_sprite.setScale(static_cast<float>(xDirection * -1), 1.0f);
		
		//Play sound
		m_changeLaneSound.stop();
		m_changeLaneSound.setPlayingOffset(sf::seconds(0.4f));
		m_changeLaneSound.play();
	}

	if (m_sandEmitter && xDirection != 0)
	{
		//Activate emitter
		float angle = (xDirection == 1) ? 0 : 180;
		m_sandEmitter->SetActive(true);
		m_sandEmitter->SetStartAngle(angle, angle);
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

	//Remove item on knockdown
	if (m_item)
	{
		m_item->SetActive(false);
		m_item->SetInGame(false);
		m_item = nullptr;
	}

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