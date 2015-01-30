#include "stdafx.h"
#include "GUI_Element.h"

GUI_Element::GUI_Element()
{
	m_x = 0;
	m_y = 0;
	m_localX = 0;
	m_localY = 0;
	m_visible = true;
	m_parent = nullptr;
}
GUI_Element::~GUI_Element()
{
	m_parent = nullptr;
}
int GUI_Element::GetX()
{
	return m_x;
}
int GUI_Element::GetY()
{
	return m_y;
}
void GUI_Element::SetPosition(int x, int y)
{
	m_localX = x;
	m_localY = y;
	Refresh();
}
void GUI_Element::Refresh()
{
	if (m_parent)
	{
		m_x = m_parent->GetX() + m_localX;
		m_y = m_parent->GetY() + m_localY;
	}
}
bool GUI_Element::IsVisible()
{
	return m_visible;
}
void GUI_Element::SetVisible(bool state)
{
	m_visible = state;
}
void GUI_Element::Update()
{
	Refresh();
}
void GUI_Element::Draw(DrawManager* drawManager)
{

}