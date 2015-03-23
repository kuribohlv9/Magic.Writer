#include "stdafx.h"
#include "Buoy.h"
#include "Animator.h"
#include "DrawManager.h"

Buoy::Buoy(sf::Texture* texture, float x, float y, const std::string animationFile)
{
	m_sprite.setTexture(*texture);
	m_sprite.setPosition(x, y);
	m_sprite.setOrigin(128, 128);
	m_animator = new Animator(&m_sprite, animationFile);
	m_animator->SetAnimation("red");
	m_animator->SetStartFrame(rand() % 6);
}

Buoy::~Buoy()
{
	if (m_animator)
	{
		delete m_animator;
		m_animator = nullptr;
	}
}

void Buoy::Update(float deltaTime)
{
	m_animator->Update(deltaTime);
}
void Buoy::Draw(DrawManager* drawManager)
{
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
}
void Buoy::SetReady(bool state)
{
	if (!state)
		m_animator->SetAnimation("red");
	else
		m_animator->SetAnimation("green");
}
