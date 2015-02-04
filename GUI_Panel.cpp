#include "stdafx.h"
#include "GUI_Panel.h"
#include "DrawManager.h"

GUI_Panel::GUI_Panel(int x, int y, GUI_Element* parent, sf::Texture* texture)
{
	if (parent)
	{
		m_parent = parent;
		m_localX = x;
		m_localY = y;

		Refresh();
	}
	else
	{
		m_localX = x;
		m_localY = y;
		m_x = m_localX;
		m_y = m_localY;
	}

	m_sprite.setTexture(*texture);
	m_sprite.setTextureRect(sf::IntRect(0, 200, 300, 300));
	m_sprite.setPosition(m_x, m_y);
}
GUI_Panel::~GUI_Panel()
{
	auto it = m_children.begin();

	while (it != m_children.end())
	{
		delete (*it);
		++it;
	}
}
void GUI_Panel::Update()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Update();
	}
}
void GUI_Panel::Draw(DrawManager* drawManager)
{
	m_sprite.setPosition(m_x, m_y);
	drawManager->Draw(m_sprite, sf::RenderStates::Default);

	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Draw(drawManager);
	}
}

void GUI_Panel::AddChild(GUI_Element* child)
{
	m_children.push_back(child);
	child->SetParent(this);
}