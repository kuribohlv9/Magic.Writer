#include "stdafx.h"
#include "Item.h"
#include "Collider.h"
#include "DrawManager.h"

Item::Item(sf::Texture* texture, sf::IntRect sourceRectangle, ItemProperties propertyOne, ItemProperties propertyTwo, const std::string& name)
{
	m_active = false;
	m_type = GAMEOBJECT_ITEM;

	m_propertyOne = propertyOne;
	m_propertyTwo = propertyTwo;

	m_name = name;
	m_speed = 150;

	m_sprite.setTexture(*texture);
	m_sprite.setTextureRect(sourceRectangle);
	sf::IntRect textureRect = m_sprite.getTextureRect();
	m_sprite.setOrigin(textureRect.width / 2, textureRect.height / 2);

	m_collider = new Collider(m_x, m_y);
	m_collider->SetParent(this);
	m_collider->SetWidthHeight(textureRect.width, textureRect.height);
}

void Item::Update(float deltaTime)
{
	Move(0, -m_speed * deltaTime);
}

std::string Item::GetName()
{
	return m_name;
}
