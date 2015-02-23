#include "stdafx.h"
#include "Monster.h"
#include "DrawManager.h"
#include "Animator.h"
#include "Collider.h"
#include <time.h>
#include "ParticleEmitter.h"

Monster::Monster(sf::Texture* texture, const std::string& animationFile, int spriteWidth, int spriteHeight, float speed, ItemProperty weakness, sf::Texture* particleTexture)
{
	m_active = false;
	srand(time(NULL));

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
	m_sprite.scale(0.5f, 0.5f);

	//Head sprite
	m_head_sprite.setTexture(*texture);
	m_head_sprite.setOrigin(m_sprite_width / 2.0f, m_sprite_height / 2.0f);
	m_head_animator = new Animator(&m_head_sprite, animationFile);
	m_head_animator->SetAnimation("move");
	m_head_sprite.scale(0.5f, 0.5f);

	//Snail sprite
	m_snail_sprite.setTexture(*texture);
	m_snail_sprite.setTextureRect(sf::IntRect(m_sprite_width, 0, m_sprite_width, m_sprite_height));
	m_snail_sprite.setOrigin(m_sprite_width / 2.0f, m_sprite_height / 2.0f);
	m_snail_sprite.setColor(sf::Color(255, 255, 255, 0));
	m_snail_sprite.scale(0.5f, 0.5f);

	//Foam sprite
	m_foam_sprite.setTexture(*texture);
	m_foam_sprite.setTextureRect(sf::IntRect(m_sprite_width * 3, 0, m_sprite_width, m_sprite_height));
	m_foam_sprite.setOrigin(m_sprite_width / 2.0f, m_sprite_height / 2.0f);
	m_foam_sprite.scale(0.5f, 0.5f);

	//Collider
	m_collider = new Collider(-20, 0);
	m_collider->SetParent(this);
	m_collider->SetWidthHeight(m_sprite_width / 4, m_sprite_height / 4);

	m_weakness = weakness;

	//Emitter test
	m_emitter = new ParticleEmitter(100, 0.1f, particleTexture);
	m_emitter->SetSize(30, 1);
	m_emitter->SetAcceleration(sf::Vector2f(0, -0.5f));
	m_emitter->SetStartVelocity(sf::Vector2f(0, -50));
	m_emitter->SetLifeTime(3);
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
		delete m_emitter;
		m_emitter = nullptr;
	}
}

void Monster::Draw(DrawManager* drawManager)
{
	m_emitter->Draw(drawManager);
	if (m_frozen)
	{
		drawManager->Draw(m_sprite, sf::RenderStates::Default);
		drawManager->Draw(m_foam_sprite, sf::RenderStates::Default);
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

	/*sf::RectangleShape shape;
	shape.setFillColor(sf::Color(0, 255, 0, 125));
	shape.setPosition(m_collider->GetArea().left, m_collider->GetArea().top);
	shape.setSize(sf::Vector2f(m_collider->GetArea().width, m_collider->GetArea().height));
	drawManager->Draw(shape, sf::RenderStates::Default);*/
}

void Monster::Update(float deltaTime)
{
	//m_emitter->Update(deltaTime);
	m_emitter->SetPosition(sf::Vector2f(GetX() - 10, GetY()));
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
		else if (m_state == MONSTER_DEATH)
		{
			if (!m_head_animator->Complete())
				m_speed = 0;
			else if (m_head_animator->Complete())
				SetActive(false);
		}
			

		//Change bodyparts scale and alpha
		HandleBodyParts();

		//Move monster position
		Move(0, m_speed * deltaTime);
		m_head_sprite.setPosition(m_sprite.getPosition());
		m_snail_sprite.setPosition(m_sprite.getPosition());
		m_foam_sprite.setPosition(m_sprite.getPosition());
	}

	//Deactivate monster
	if (m_y > ScreenHeight)
	{
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
			m_sprite.setTextureRect(sf::IntRect(0, 0, m_sprite_width, m_sprite_height));
		}
	}
	else
	{
		m_frozen = false;
		m_sprite.setTextureRect(sf::IntRect(m_sprite_width * 2, 0, m_sprite_width, m_sprite_height));
	}
}
bool Monster::Burst()
{
	if (!m_activeBurst)
	{
		m_activeBurst = true;
		m_collider->SetWidthHeight(0, 0);
		m_speed = 400;
		return true;
	}
	return false;
}

void Monster::Damage(ItemProperty property, int &score)
{
	if (property == m_weakness)
	{
		m_health -= 2;
		score += 500;
		m_state = MONSTER_CRITICAL;
		m_head_animator->SetAnimation("critical");
		Move(0, -75);
	}
	else
	{
		m_health -= 1;
		score += 250;
		m_state = MONSTER_HIT;
		m_head_animator->SetAnimation("hit");
	}

	if (m_health <= 0)
	{
		m_state = MONSTER_DEATH;
		m_head_animator->SetAnimation("death");
	}
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

	float scale = (800 - (800 - m_y - 200)) / 800;

	if (scale > 1)
		scale = 1;
	else if (scale < 0.3f)
		scale = 0.3f;
	m_sprite.setScale(scale, scale);
	m_head_sprite.setScale(scale, scale);
	m_foam_sprite.setScale(scale, scale);
}

void Monster::Activate()
{
	//Activation and reset function
	m_frozen = false;
	m_speed = m_originalSpeed;
	m_health = 3;


	m_foam_sprite.setColor(sf::Color(255, 255, 255, 255));
	m_snail_sprite.setColor(sf::Color(255, 255, 255, 0));


	//Randomize start position
	int randomLane = rand() % 5;
	SetPosition(Lanes[randomLane], -m_sprite_height);

	//Reset collider
	m_collider->SetWidthHeight(m_sprite_width / 4, m_sprite_height / 4);

	SetActive(true);
}