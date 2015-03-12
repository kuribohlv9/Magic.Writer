#include "stdafx.h"
#include "Wave.h"
#include "DrawManager.h"
#include "Animator.h"
#include <iostream>

Wave::Wave(sf::Texture* texture)
{
	m_sprite.setTexture(*texture);
	m_speed = 50;
	SetActive(false);

	m_animator = new Animator(&m_sprite, "assets/sprites/wave_spritesheet.txt");
	m_animator->SetAnimation("wave_animation");
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2, m_sprite.getTextureRect().height / 2);
	SetPosition(ScreenWidth / 2, 0);
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

	float distance = abs(GetY() - 800.0f);
	float scaleFactor = distance / 800.0f;
	m_sprite.setScale(1.8f - scaleFactor, 1.0f);


	float alphaDistance = abs(GetY() - 650.0f);
	float alpha = 255 * (alphaDistance / 650.0f);
	m_sprite.setColor(sf::Color(255, 255, 255, alpha));

	if (GetY() >= 650.0f)
		SetActive(false);

}
void Wave::Draw(DrawManager* drawManager)
{
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
}

void Wave::Activate()
{
	m_sprite.setColor(sf::Color(255, 255, 255, 125));
	m_sprite.setScale(1, 1);
	SetPosition(GetX(), -100);
	SetActive(true);
}
