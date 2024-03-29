#include "stdafx.h"
#include "Monster.h"
#include "DrawManager.h"
#include "Animator.h"
#include "Collider.h"
#include <time.h>
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "TextureManager.h"
#include "ServiceLocator.h"
#include "AudioManager.h"

Monster::Monster(sf::Texture* texture, const std::string& animationFile, int spriteWidth, int spriteHeight, float speed, ItemProperty weakness, sf::Texture* particleTexture)
{
	m_textureManager = ServiceLocator<TextureManager>::GetService();

	m_active = false;

	//Variables
	m_type = GAMEOBJECT_MONSTERS;
	m_state = MONSTER_MOVE;
	m_speed = speed;
	m_originalSpeed = m_speed;
	m_health = 3;
	m_frozen = false;

	//Calculate sprite width and height
	m_sprite_width = spriteWidth;
	m_sprite_height = spriteHeight;

	//Sprite
	m_sprite.setTexture(*texture);
	m_sprite.setTextureRect(sf::IntRect(m_sprite_width * 2, 0, m_sprite_width, m_sprite_height));
	m_sprite.setOrigin(m_sprite_width / 2.0f, m_sprite_height / 2.0f);

	//Head sprite
	m_head_sprite.setTexture(*texture);
	m_head_sprite.setOrigin(m_sprite_width / 2.0f, m_sprite_height / 2.0f);
	m_head_animator = new Animator(&m_head_sprite, animationFile);
	m_head_animator->SetAnimation("move");

	//Snail sprite
	m_snail_sprite.setTexture(*texture);
	m_snail_sprite.setTextureRect(sf::IntRect(m_sprite_width, 0, m_sprite_width, m_sprite_height));
	m_snail_sprite.setOrigin(m_sprite_width / 2.0f, m_sprite_height / 2.0f);
	m_snail_sprite.setColor(sf::Color(255, 255, 255, 0));

	//Foam sprite
	m_foam_sprite.setTexture(*texture);
	m_foam_sprite.setTextureRect(sf::IntRect(m_sprite_width * 3, 0, m_sprite_width, m_sprite_height));
	m_foam_sprite.setOrigin(m_sprite_width / 2.0f, m_sprite_height / 2.0f);

	//Collider
	m_collider = new Collider(-20, 0);
	m_collider->SetParent(this);
	m_collider->SetWidthHeight(m_sprite_width / 4, m_sprite_height / 4);

	sf::Texture* texturE = m_textureManager->LoadTexture("assets/sprites/monster/life_bar.png");
	m_lifeBar.setTexture(*texturE);

	m_weakness = weakness;

	m_emitter = ServiceLocator<ParticleManager>::GetService()->CreateEmitter(particleTexture, 50);
	m_emitter->SetLifeTime(0.7f, 0.7f);
	m_emitter->SetSpawnRate(0.1f);
	m_emitter->SetPosition(-100, -100);
	m_emitter->SetForce(150, 150);
	m_emitter->SetAcceleration(0, 0);
	m_emitter->SetStartAngle(270, 270);
	m_emitter->SetSize(20, 1);
}
Monster::~Monster()
{
	if (m_head_animator)
	{
		delete m_head_animator;
		m_head_animator = nullptr;
	}
	if (m_emitter)
	{
		m_emitter = nullptr;
	}
}

void Monster::Draw(DrawManager* drawManager)
{
	if (m_frozen)
	{
		if (m_state == MONSTER_DEATH)
			drawManager->Draw(m_head_sprite, sf::RenderStates::Default);
		else
		{
			drawManager->Draw(m_sprite, sf::RenderStates::Default);
			drawManager->Draw(m_foam_sprite, sf::RenderStates::Default);
		}
	}
	else
	{
		drawManager->Draw(m_snail_sprite, sf::RenderStates::Default);
		drawManager->Draw(m_sprite, sf::RenderStates::Default);
		drawManager->Draw(m_foam_sprite, sf::RenderStates::Default);
		drawManager->Draw(m_head_sprite, sf::RenderStates::Default);
	}

	//Draw health
	for (int i = 0; i < m_health; i++)
	{
		m_lifeBar.setPosition(m_x + i*25 - 50, m_y-120);
		drawManager->Draw(m_lifeBar, sf::RenderStates::Default);
	}

	/*sf::RectangleShape shape;
	shape.setFillColor(sf::Color(0, 255, 0, 125));
	shape.setPosition(m_collider->GetArea().left, m_collider->GetArea().top);
	shape.setSize(sf::Vector2f(m_collider->GetArea().width, m_collider->GetArea().height));
	drawManager->Draw(shape, sf::RenderStates::Default);*/
}

void Monster::Update(float deltaTime)
{
	m_emitter->SetPosition(GetX() - 20, GetY());

	m_totalLifeTime += deltaTime;

	if (!m_frozen)
	{
		//Update animation
		m_head_animator->Update(deltaTime);
		if (m_state == MONSTER_HIT || m_state == MONSTER_CRITICAL)
		{
			if (m_head_animator->Complete())
			{
				m_state = MONSTER_MOVE;
				m_head_animator->SetAnimation("move");
			}
		}
		else if (m_head_animator->Complete())
		{
			SetActive(false);
		}
			

		//Change bodyparts scale and alpha
		HandleBodyParts();

		//Move monster position
		if (m_state == MONSTER_HIT || m_state == MONSTER_DEATH)
			Move(0, -m_speed * deltaTime * 3);
		else if (m_state == MONSTER_CRITICAL)
			Move(0, -m_speed * deltaTime * 5);
		else
			Move(0, m_speed * deltaTime);

		m_head_sprite.setPosition(m_sprite.getPosition());
		m_snail_sprite.setPosition(m_sprite.getPosition());
		m_foam_sprite.setPosition(m_sprite.getPosition());

		//Deactivate monster
		if (m_y > ScreenHeight)
		{
			SetActive(false);
		}
	}
	else if (m_state == MONSTER_DEATH)
	{
		m_head_animator->Update(deltaTime);
		if (m_head_animator->Complete())
			SetActive(false);
	}
}

void Monster::Freeze(bool state)
{
	if (state)
	{
		if (m_y < 700)
		{
			m_frozen = true;
			m_emitter->SetActive(false);
			m_sprite.setTextureRect(sf::IntRect(0, 0, m_sprite_width, m_sprite_height));
		}
	}
	else
	{
		if (m_state != MONSTER_DEATH)
			m_emitter->SetActive(true);
		m_frozen = false;
		m_sprite.setTextureRect(sf::IntRect(m_sprite_width * 2, 0, m_sprite_width, m_sprite_height));
	}
}
bool Monster::Burst()
{
	if (!m_activeBurst)
	{
		m_activeBurst = true;
		m_emitter->SetActive(false);
		m_collider->SetWidthHeight(0, 0);
		m_speed = 400;
		return true;
	}
	return false;
}
bool Monster::IsDead()
{
	return m_health <= 0;
}

void Monster::Damage(ItemProperty property, bool& oneShot)
{ 
	oneShot = false;
	int randomSoundIndex = rand() % 3;
	m_hitSound.setVolume(100);
	m_hitSound.setBuffer(*m_hitBuffers[randomSoundIndex]);

	if (property == m_weakness)
	{
		oneShot = (m_health == 2);

		m_health -= 2;
		m_state = MONSTER_CRITICAL;
		m_head_animator->SetAnimation("critical");
	}
	else if (property == ITEM_NEUTRAL)
	{
		m_health -= 1;
		m_state = MONSTER_HIT;
		m_head_animator->SetAnimation("hit");
	}
	else
	{
		m_hitSound.setVolume(10);
		m_hitSound.setBuffer(*m_hitBuffers[3]);
	}

	if (m_health <= 0)
	{
		m_state = MONSTER_DEATH;
		m_head_animator->SetAnimation("death");
		m_collider->SetWidthHeight(0, 0);
		m_emitter->SetActive(false);
		m_hitSound.setVolume(10);
		m_hitSound.setBuffer(*m_cheering);

		m_foam_sprite.setColor(sf::Color::Transparent);
		m_sprite.setColor(sf::Color::Transparent);
		m_snail_sprite.setColor(sf::Color::Transparent);
	}
	m_hitSound.play();
}

void Monster::HandleBodyParts()
{
	//Change snails y scale
	float y_scale = 1.0f + abs(0.1f * cos(m_totalLifeTime * 3.0f));
	if (y_scale >= 1.3f)
		y_scale = 1.3f;
	m_snail_sprite.setScale(1, y_scale);

	//Change snails color
	if (m_y >= 500)
	{
		sf::Color snail_color = m_snail_sprite.getColor();

		float snail_alpha = snail_color.a;
		snail_alpha += 1.5f;

		if (snail_alpha > 255)
			snail_alpha = 255;

		snail_color.a = static_cast<sf::Uint8>(snail_alpha);
		m_snail_sprite.setColor(snail_color);
	}

	//Change foam color
	if (m_y >= 550)
	{
		sf::Color foam_color = m_foam_sprite.getColor();

		int foam_alpha = foam_color.a;
		foam_alpha -= 2;

		if (foam_alpha < 0)
			foam_alpha = 0;

		foam_color.a = foam_alpha;
		m_foam_sprite.setColor(foam_color);
	}
}

void Monster::Activate(int speed)
{
	//Activation and reset function
	m_frozen = false;
	m_activeBurst = false;
	m_speed = (float)speed;
	m_health = 2;
	m_state = MONSTER_MOVE;

	//Set color
	m_foam_sprite.setColor(sf::Color::White);
	m_snail_sprite.setColor(sf::Color(255, 255, 255, 0));
	m_sprite.setColor(sf::Color::White);
	m_sprite.setTextureRect(sf::IntRect(m_sprite_width * 2, 0, m_sprite_width, m_sprite_height));


	//Randomize start position
	int randomLane = rand() % 5;
	SetPosition(Lanes[randomLane], static_cast<float>(-m_sprite_height));

	//Reset collider
	m_collider->SetWidthHeight(m_sprite_width / 4, m_sprite_height / 4);

	//Set animation
	m_head_animator->SetAnimation("move");
	SetActive(true);
	m_emitter->SetActive(true);
}

void Monster::SetSounds(sf::SoundBuffer* hitBuffer, sf::SoundBuffer* hitBufferTwo, sf::SoundBuffer* hitBufferThree, sf::SoundBuffer* hitBufferFour)
{
	m_hitBuffers[0] = hitBuffer;
	m_hitBuffers[1] = hitBufferTwo;
	m_hitBuffers[2] = hitBufferThree;
	m_hitBuffers[3] = hitBufferFour;

	m_hitSound.setVolume(10);

	m_cheering = ServiceLocator<AudioManager>::GetService()->LoadSoundFromFile("assets/audio/complete/small_exterior_crowd_applause_with_cheering_fix.wav");
}