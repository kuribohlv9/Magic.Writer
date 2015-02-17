#include "stdafx.h"
#include "Item.h"
#include "Collider.h"
#include "DrawManager.h"

Item::Item(sf::Texture* texture, sf::IntRect sourceRectangle, ItemProperty property, const std::string& name)
{
	m_active = false;
	m_type = GAMEOBJECT_ITEM;

	m_property = property;

	m_name = name;
	m_inGame = false;
	m_speed = 400;

	m_sprite.setTexture(*texture);
	m_sprite.setTextureRect(sourceRectangle);
	sf::IntRect textureRect = m_sprite.getTextureRect();
	m_sprite.setOrigin(textureRect.width / 2.0f, textureRect.height / 2.0f);

	m_collider = new Collider(m_x, m_y);
	m_collider->SetParent(this);
	m_collider->SetWidthHeight(textureRect.width * 0.4f, textureRect.height * 0.4f);
}

void Item::Update(float deltaTime)
{
	//Move upwards
	Move(0, -m_speed * deltaTime);

	//Rotate item
	m_rotation += 5.0f;
	if (m_rotation >= 360)
		m_rotation = 0;
	m_sprite.setRotation(m_rotation);

	//Deactivate above screen
	if (m_y <= -50)
	{
		SetActive(false);
		SetInGame(false);
	}
}

std::string Item::GetName()
{
	return m_name;
}
ItemProperty Item::GetProperty()
{
	return m_property;
}

bool Item::IsInGame()
{
	return m_inGame;
}
void Item::SetInGame(bool state)
{
	if (state)
		Reset();

	m_inGame = state;
}

void Item::ActivateItem()
{
	m_sprite.setScale(1, 1);
	SetActive(true);
}

void Item::Reset()
{
	m_rotation = 0;
	m_sprite.setRotation(m_rotation);

	m_sprite.setScale(0.5, 0.5);
}