#include "stdafx.h"
#include "Buoy.h"
#include "Animator.h"
#include "DrawManager.h"

Buoy::Buoy(sf::Texture* texture, float x, float y)
{
	m_sprite.setTexture(*texture);
	m_sprite.setPosition(x, y);
	m_sprite.setOrigin(64, 64);
	m_animator = new Animator(&m_sprite, "assets/sprites/buoy_animation.txt");
	m_animator->SetAnimation("float");
	m_animator->SetStartFrame(rand() % 6);
}

Buoy::~Buoy()
{
}

void Buoy::Update(float deltaTime)
{
	m_animator->Update(deltaTime);
}
void Buoy::Draw(DrawManager* drawManager)
{
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
}
void Buoy::SwitchFlag()
{

}
