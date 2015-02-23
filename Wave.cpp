#include "stdafx.h"
#include "Wave.h"
#include "DrawManager.h"
#include "Animator.h"

Wave::Wave(sf::Texture* texture)
{
	m_sprite.setTexture(*texture);
	m_speed = 300.0f;

	m_animator = new Animator(&m_sprite, "assets/sprites/wave_spritesheet.txt");
	m_animator->SetAnimation("wave_animation");
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
}
void Wave::Draw(DrawManager* drawManager)
{
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
}
