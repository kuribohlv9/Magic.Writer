#include "stdafx.h"
#include "Bubble.h"
#include "DrawManager.h"
#include "Item.h"

Bubble::Bubble(float x, float y, sf::Texture* texture)
{
	m_sprite.setPosition(x, y);
	m_width = texture->getSize().x / 4;
	m_height = texture->getSize().y;

	m_sprite.setTexture(*texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_width, m_height));
	m_sprite.setOrigin(m_width / 2, m_height / 2);

	m_item = nullptr;
}

void Bubble::SetItem(Item* item)
{
	m_item = item;
	m_item->SetPosition(m_sprite.getPosition().x, m_sprite.getPosition().y - 10);

	int x = (int)(item->GetProperty());

	m_sprite.setTextureRect(sf::IntRect(x * m_width, 0, m_width, m_height));
}
Item* Bubble::GetItem()
{
	return m_item;
}
void Bubble::Draw(DrawManager* drawManager)
{
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
	if (m_item)
		m_item->Draw(drawManager);
}