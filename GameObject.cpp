//GameObjects.cpp

#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject()
{
	// Default values.
	m_x = 0;
	m_y = 0;
	m_active = true;
	m_collider = nullptr;
}

GameObject::~GameObject()
{
	if (m_collider)
		delete m_collider;
}
void GameObject::Update(float delaTime)
{

}
void GameObject::Draw(sf::RenderWindow* window)
{
	window->draw(m_sprite, sf::RenderStates::Default);
}
GType GameObject::GetType()
{
	return m_type;
};
sf::Sprite* GameObject::GetSprite()
{
	return &m_sprite;
}

Collider* GameObject::GetCollider()
{
	return m_collider;
}

bool GameObject::IsActive()
{
	return m_active;
}

float GameObject::GetX()
{
	return m_x;
}

float GameObject::GetY()
{
	return m_y;
}