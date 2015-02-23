#include "stdafx.h"
#include "GUI_Button.h"
#include "GUI_Label.h"
#include "DrawManager.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "InputManager.h"
#include "ServiceLocator.h"

GUI_Button::GUI_Button(int x, int y, GUI_Element* parent, sf::Texture* texture, sf::IntRect sourceRectangle)
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

	m_buttonRectangle = sourceRectangle;
	m_sprite.setTexture(*texture);
	m_sprite.setTextureRect(sourceRectangle);
	m_sprite.setPosition(m_x, m_y);

	m_collider = new Collider(0, 0);
	m_collider->SetWidthHeight(m_buttonRectangle.width, m_buttonRectangle.height);
	m_inputManager = ServiceLocator<InputManager>::GetService();

}
GUI_Button::~GUI_Button()
{
	if (m_label)
	{
		delete m_label;
		m_label = nullptr;
	}

	if (m_collider)
	{
		delete m_collider;
		m_collider = nullptr;
	}

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
			sf::IntRect rect = m_buttonRectangle;
			rect.top += rect.height;
			m_sprite.setTextureRect(rect);
		}
		else
		{
			m_sprite.setTextureRect(m_buttonRectangle);
		}
		drawManager->Draw(m_sprite, sf::RenderStates::Default);

		if (m_label)
		{
			m_label->Draw(drawManager);
		}
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