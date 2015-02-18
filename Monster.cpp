#include "stdafx.h"
#include "Monster.h"
#include "DrawManager.h"
#include "Animator.h"
#include "Collider.h"
#include <time.h>

Monster::Monster(sf::Texture* texture, float speed, int health, ItemProperty weakness)
{
	m_active = false;
	srand(time(NULL));
	//Variables
	m_type = GAMEOBJECT_MONSTERS;
	m_state = MONSTER_MOVE;
	m_speed = speed;
	m_health = health;
	m_frozen = false;

	//Calculate sprite width and height
	m_sprite_width = 207;
	m_sprite_height = 207;

	//Sprite
	m_sprite.setTexture(*texture);
	m_sprite.setTextureRect(sf::IntRect(m_sprite_width * 2, 0, m_sprite_width, m_sprite_height));
	m_sprite.setOrigin(m_sprite_width / 2.0f, m_sprite_height / 2.0f);

	//Head sprite
	m_head_sprite.setTexture(*texture);
	m_head_sprite.setOrigin(m_sprite_width / 2.0f, m_sprite_height / 2.0f);
	m_head_animator = new Animator(&m_head_sprite, "assets/sprites/monster/monster_animation.txt");
	m_head_animator->SetAnimation("move");

	//Snail sprite
	m_snail_sprite.setTexture(*texture);
	m_snail_sprite.setTextureRect(sf::IntRect(m_sprite_width, 0, m_sprite_width, m_sprite_height));
	m_snail_sprite.setOrigin(m_sprite_width / 2.0f, m_sprite_height / 2.0f);
	m_snail_sprite.setColor(sf::Color(255, 255, 255, 0));

	//Foeam sprite
	m_foam_sprite.setTexture(*texture);
	m_foam_sprite.setTextureRect(sf::IntRect(m_sprite_width * 3, 0, m_sprite_width, m_sprite_height));
	m_foam_sprite.setOrigin(m_sprite_width / 2.0f, m_sprite_height / 2.0f);

	//Collider
	m_collider = new Collider(0, 0);
	m_collider->SetParent(this);
	m_collider->SetWidthHeight(m_sprite_width / 2, m_sprite_height / 2);

	//Randomize start position
	int randomLane = rand() % 5;
	int laneWidth = ScreenWidth / 5;
	int xPosition = laneWidth / 2 + laneWidth * randomLane;
	SetPosition(xPosition, -m_sprite_height);

	////Weakness
	//int randomWeakness = rand() % 4;
	//ItemProperty weakness;
	//switch (randomWeakness)
	//{
	//case 0:
	//	weakness = ITEM_ALIVE;
	//	m_color = sf::Color(80, 80, 80, 255);
	//	break;
	//case 1:
	//	weakness = ITEM_DEAD;
	//	m_color = sf::Color::White;
	//	break;
	//case 2:
	//	weakness = ITEM_COLD;
	//	m_color = sf::Color(150, 50, 50, 255);
	//	break;
	//case 3:
	//	weakness = ITEM_HOT;
	//	m_color = sf::Color(50, 50, 150, 255);
	//	break;
	//}

	m_sprite.setColor(m_color);
	m_head_sprite.setColor(m_color);
	m_weakness = weakness;
}
Monster::~Monster()
{
	if (m_head_animator)
	{
		delete m_head_animator;
	}
}

void Monster::Draw(DrawManager* drawManager)
{
	if (!m_frozen)
	{
		if (m_frozen)
		{
			drawManager->Draw(m_sprite, sf::RenderStates::Default);
		}
		else
		{
			drawManager->Draw(m_snail_sprite, sf::RenderStates::Default);
			drawManager->Draw(m_sprite, sf::RenderStates::Default);
			drawManager->Draw(m_foam_sprite, sf::RenderStates::Default);
			drawManager->Draw(m_head_sprite, sf::RenderStates::Default);
		}

		//Draw health
		sf::CircleShape hp;
		for (int i = 0; i < m_health; i++)
		{
			hp.setFillColor(sf::Color::White);
			hp.setRadius(15);
			hp.setPosition(m_x + i*30, m_y-100);
			drawManager->Draw(hp, sf::RenderStates::Default);
			hp.setFillColor(sf::Color::Black);
			hp.setRadius(10);
			drawManager->Draw(hp, sf::RenderStates::Default);
		}
	}
}

void Monster::Update(float deltaTime)
{
	m_totalLifeTime += deltaTime;

	if (!m_frozen)
	{
		if (m_state == MONSTER_HIT || m_state == MONSTER_CRITICAL)
		{
			if (m_head_animator->Complete())
			{
				m_state = MONSTER_MOVE;
				m_head_animator->SetAnimation("move");
			}
		}

		m_head_animator->Update(deltaTime);
		Move(0, m_speed * deltaTime);

		m_head_sprite.setPosition(m_sprite.getPosition());
		m_snail_sprite.setPosition(m_sprite.getPosition());
		m_foam_sprite.setPosition(m_sprite.getPosition());

		//Change snails y scale
		float y_scale = 1.0f + abs(0.1f * cos(m_totalLifeTime * 3.0f));
		if (y_scale >= 1.3f)
			y_scale = 1.3f;
		m_snail_sprite.setScale(1, y_scale);

		//Change snails color
		if (m_y >= 500)
		{
			sf::Color snail_color = m_snail_sprite.getColor();

			int snail_alpha = snail_color.a;
			snail_alpha += 1.5f;

			if (snail_alpha > 255)
				snail_alpha = 255;

			snail_color.a = snail_alpha;
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
	else
	{
		m_freezeTimer += deltaTime;

		if (m_freezeTimer >= m_unfreezeDelay)
		{
			m_freezeTimer = 0;
			m_unfreezeDelay = 0;
			m_frozen = false;
			m_sprite.setTextureRect(sf::IntRect(m_sprite_width * 2, 0, m_sprite_width, m_sprite_height));
		}
	}

	if (m_y > ScreenHeight)
	{
		m_active = false;
	}
}

void Monster::Freeze(float time)
{
	m_unfreezeDelay = time;
	m_frozen = true;
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_sprite_width, m_sprite_height));
}

void Monster::Damage(ItemProperty property, int &score)
{
	if (property == m_weakness)
	{
		m_health -= 2;
		score += 5000;
		m_state = MONSTER_CRITICAL;
		m_head_animator->SetAnimation("critical");
	}
	else
	{
		m_health -= 1;
		score += 2500;
		m_state = MONSTER_HIT;
		m_head_animator->SetAnimation("hit");
	}

	if (m_health <= 0)
	{
		SetActive(false);
	}
}

void Monster::Activate()
{
	m_active = true;
}