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
	m_speed = 5.0f;

	m_sprite.setTexture(*texture);
	m_sprite.setTextureRect(sourceRectangle);

	m_collider = new Collider(m_x, m_y);
	m_collider->SetParent(this);
	m_collider->SetWidthHeight(m_sprite.getTextureRect().width, m_sprite.getTextureRect().height);
}

void Item::Update(float deltaTime)
{
	Move(0, -m_speed * deltaTime);
}
void Item::Draw(DrawManager* drawManager)
{
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
}

void Item::Activate(float x, float y)
{
	SetPosition(x, y);
	SetActive(true);
}
