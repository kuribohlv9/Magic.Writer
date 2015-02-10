#include "stdafx.h"
#include "InputManager.h"
#include "Collider.h"
InputManager::InputManager()
{

}
InputManager::~InputManager()
{

}
bool InputManager::Initialize()
{
	m_mouseCollider = new Collider(m_mouse_position.x, m_mouse_position.y);
	m_mouseCollider->SetWidthHeight(1, 1);
	m_lastInputChar = ' ';
	return true;
}
void InputManager::Shutdown()
{
	delete m_mouseCollider;
}

bool InputManager::IsKeyDown(sf::Keyboard::Key key)
{
	m_keys[key] = sf::Keyboard::isKeyPressed(key);
	return m_keys[key];
}
bool InputManager::IsKeyDownOnce(sf::Keyboard::Key key)
{
	//Returns true if key is not pressed the last frame and pressed the current frame
	return (m_keys[key] && !m_lastKeys[key]);
}

bool InputManager::IsButtonDown(sf::Mouse::Button button)
{
	m_buttons[button] = sf::Mouse::isButtonPressed(button);
	return m_buttons[button];
}
bool InputManager::IsButtonDownOnce(sf::Mouse::Button button)
{
	//Returns true if button is not pressed the last frame and pressed the current frame
	return (m_buttons[button] && !m_lastButtons[button]);
}

void InputManager::SetLastInputs()
{
	//Copies every m_keys to m_lastKeys in order to save what keys were pressed in the last frame
	for (unsigned int i = 0; i < sizeof(m_keys) / sizeof(m_keys[0]); i++)
	{
		m_lastKeys[i] = m_keys[i];
	}

	//Copies every m_buttons to m_lastButtons in order to save what buttons were pressed in the last frame
	for (unsigned int i = 0; i < sizeof(m_buttons) / sizeof(m_buttons[0]); i++)
	{
		m_lastButtons[i] = m_buttons[i];
	}
}

void InputManager::SetButton(int button, bool state)
{
	m_buttons[button] = state;
}
void InputManager::SetKey(int key, bool state)
{
	m_keys[key] = state;
}

void InputManager::SetMousePosition(int x, int y)
{
	m_mouse_position.x = x;
	m_mouse_position.y = y;

	m_mouseCollider->SetPosition(m_mouse_position.x, m_mouse_position.y);
}
sf::Vector2f InputManager::GetMousePosition()
{
	return m_mouse_position;
}
Collider* InputManager::GetMouseCollider()
{
	return m_mouseCollider;
}
char InputManager::GetInputChar()
{
	return m_lastInputChar;
}
void InputManager::SetInputChar(char key)
{
	m_lastInputChar = key;
}