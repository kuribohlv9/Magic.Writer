#include "stdafx.h"
#include "GameObject.h"
#include "Collider.h"

Collider::Collider(int xOffset, int yOffset)
{
	m_parent = nullptr;
	m_xOffset = xOffset;
	m_yOffset = yOffset;

	m_area.left = 0;
	m_area.top = 0;
	m_area.width = 0;
	m_area.height = 0;
}

bool Collider::HasParent()
{
	return m_parent != nullptr;
}

void Collider::SetParent(GameObject* parent)
{
	m_parent = parent;
}

GameObject* Collider::GetParent()
{
	return m_parent;
}

void Collider::SetPosition(float x, float y)
{
	m_area.left = x;
	m_area.top = y;
}

void Collider::SetWidthHeight(int width, int height)
{
	m_area.width = width;
	m_area.height = height;
}

sf::FloatRect Collider::GetArea()
{
	return m_area;
}

void Collider::Refresh()
{
	m_area.left = m_parent->GetX() - m_area.width / 2 + m_xOffset;
	m_area.top = m_parent->GetY() - m_area.height / 2 + m_yOffset;
}