#include "stdafx.h"
#include "GUI_Label.h"
#include "DrawManager.h"

GUI_Label::GUI_Label(float x, float y, GUI_Element* parent, sf::Font* font, std::string text)
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

	m_text.setFont(*font);
	m_text.setString(text);
	m_text.setPosition(m_x, m_y);
}

sf::Text* GUI_Label::GetText()
{
	return &m_text;
}
void GUI_Label::SetString(const std::string& text)
{
	m_text.setString(text);
}
void GUI_Label::SetColor(const sf::Color& color)
{
	m_text.setColor(color);
}
void GUI_Label::Draw(DrawManager* drawManager)
{
	if (m_visible)
	{
		drawManager->Draw(m_text, sf::RenderStates::Default);
	}
}
void GUI_Label::Refresh()
{
	if (m_parent)
	{
		m_x = m_parent->GetX() + m_localX;
		m_y = m_parent->GetY() + m_localY;
	}
	m_text.setPosition(m_x, m_y);
}