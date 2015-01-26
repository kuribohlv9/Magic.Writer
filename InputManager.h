#pragma once

//Saves current frames keypresses
static bool m_keys[255];
//Saves last frames keypresses
static bool m_lastKeys[255];

//Saves current frames buttonpresses
static bool m_buttons[3];
//Saves last frames buttonpresses
static bool m_lastButtons[3];

class InputManager
{
public:
	static bool IsKeyDown(sf::Keyboard::Key key);
	static bool IsKeyDownOnce(sf::Keyboard::Key key);

	static bool IsButtonDown(sf::Mouse::Button button);
	static bool IsButtonDownOnce(sf::Mouse::Button button);

	static void SetKey(int key, bool state);
	static void SetButton(int button, bool state);
	static void SetLastInputs();
};