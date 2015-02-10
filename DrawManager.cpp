#include "stdafx.h"
#include "DrawManager.h"

DrawManager::DrawManager(sf::RenderWindow* window)
{
	m_window = window;
}
DrawManager::~DrawManager()
{

}

bool DrawManager::Initialize()
{
	return true;
}
void DrawManager::Shutdown()
{
		if (m_window)
	{
		delete m_window;
		m_window = nullptr;
	}
}

void DrawManager::Draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
	m_window->draw(drawable, states);
}