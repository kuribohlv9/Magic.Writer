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
	m_sprite.setScale(0.6f, 0.6f);
	sf::IntRect textureRect = m_sprite.getTextureRect();
	m_sprite.setOrigin(textureRect.width / 2, textureRect.height / 2);

	m_collider = new Collider(m_x, m_y);
	m_collider->SetParent(this);
	m_collider->SetWidthHeight(textureRect.width * 0.4f, textureRect.height * 0.4f);
}

void Item::Update(float deltaTime)
{
	Move(0, -m_speed * deltaTime);

	//TMP
	m_rotation += 0.5f;
	if (m_rotation >= 360)
		m_rotation = 0;
	m_sprite.setRotation(m_rotation);
}

std::string Item::GetName()
{
	return m_name;
}
