#include "stdafx.h"
#include "GUI_Button.h"
#include "GUI_Label.h"
#include "DrawManager.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "InputManager.h"
#include "ServiceLocator.h"

GUI_Button::GUI_Button(int x, int y, GUI_Element* parent, sf::Texture* texture, int buttonWidth, int buttonHeight)
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

	m_buttonWidth = buttonWidth;
	m_buttonHeight = buttonHeight;
	m_sprite.setTexture(*texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_buttonWidth, m_buttonHeight));
	m_sprite.setPosition(m_x, m_y);

	m_collider = new Collider(0, 0);
	m_collider->SetWidthHeight(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height);
	m_inputManager = ServiceLocator<InputManager>::GetService();

}
GUI_Button::~GUI_Button()
{
	delete m_label;
	m_label = nullptr;

	delete m_collider;
	m_collider = nullptr;

	m_inputManager = nullptr;
}

void GUI_Button::SetLabel(GUI_Label* label)
{
	m_label = label;
}
void GUI_Button::SetParent(GUI_Element* parent)
{
	m_parent = parent;
}
void GUI_Button::Update()
{
	m_hovered = false;
	m_pressed = false;
	if (CollisionManager::Check(m_collider, m_inputManager->GetMouseCollider()))
	{
		m_hovered = true;
		if (m_inputManager->IsButtonDownOnce(sf::Mouse::Left))
		{
			m_pressed = true;
		}
	}
	Refresh();

	if (m_label)
	{
		m_label->Update();
	}
}
bool GUI_Button::IsPressed()
{
	return m_pressed;
}
void GUI_Button::Draw(DrawManager* drawManager)
{
	if (m_visible)
	{
		if (m_hovered)
		{
			m_sprite.setTextureRect(sf::IntRect(0, m_buttonHeight, m_buttonWidth, m_buttonHeight));
		}
		else
		{
			m_sprite.setTextureRect(sf::IntRect(0, 0, m_buttonWidth, m_buttonHeight));
		}
		drawManager->Draw(m_sprite, sf::RenderStates::Default);
		m_label->Draw(drawManager);
	}
}
GUI_Label* GUI_Button::GetLabel()
{
	return m_label;
}
sf::Sprite* GUI_Button::GetSprite()
{
	return &m_sprite;
}
void GUI_Button::Refresh()
{
	if (m_parent)
	{
		m_x = m_parent->GetX() + m_localX;
		m_y = m_parent->GetY() + m_localY;
	}
	m_collider->SetPosition(m_x, m_y);
	m_sprite.setPosition(m_x, m_y);
}