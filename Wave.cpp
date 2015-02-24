#include "stdafx.h"
#include "Wave.h"
#include "DrawManager.h"
#include "Animator.h"

Wave::Wave(sf::Texture* texture)
{
	m_sprite.setTexture(*texture);
	m_speed = 50;
	SetActive(false);


	m_animator = new Animator(&m_sprite, "assets/sprites/wave_spritesheet.txt");
	m_animator->SetAnimation("wave_animation");
	m_sprite.setScale(0.5f, 0.5f);
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2, m_sprite.getTextureRect().height / 2);

	SetPosition(rand() % ScreenWidth, -50);
}
Wave::~Wave()
{
	if (m_animator)
	{
		delete m_animator;
		m_animator = nullptr;
	}
}

void Wave::Update(float deltaTime)
{
	m_animator->Update(deltaTime);
	Move(0, m_speed * deltaTime);

	if (GetY() > ScreenHeight)
	{
		SetActive(false);
	}

	float scale = (800 - (800 - m_y - 200)) / 800;

	if (scale > 1)
		scale = 1;
	else if (scale < 0.4f)
		scale = 0.4f;
	m_sprite.setScale(scale, scale);
}
void Wave::Draw(DrawManager* drawManager)
{
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
}
