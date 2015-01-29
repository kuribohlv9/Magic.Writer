#include "stdafx.h"
#include "Monster.h"
#include "DrawManager.h"
#include "Collider.h"

// INTE KLAR HÄR ÄN!!!

Monster::Monster(sf::Texture* texture, int x, int y)
{
	m_type = ENTITY_MONSTERS;

	m_x = x;
	m_y = y;

	m_sprite.setTexture(*texture);
	m_collider = new Collider(m_x, m_y);
	m_collider->SetParent(this);
	m_collider->SetWidthHeight(m_sprite.getTextureRect().width, m_sprite.getTextureRect().height);
}

Monster::~Monster()
{

}

void Monster::Draw(DrawManager* drawManager)
{
	m_sprite.setPosition(m_x, m_y);
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
}

void Monster::Update(float deltaTime)
{

}