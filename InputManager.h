#pragma once

class Collider;

class InputManager
{
public:
	bool Initialize();
	void Shutdown();

	bool IsKeyDown(sf::Keyboard::Key key);
	bool IsKeyDownOnce(sf::Keyboard::Key key);

	bool IsButtonDown(sf::Mouse::Button button);
	bool IsButtonDownOnce(sf::Mouse::Button button);

	void SetKey(int key, bool state);
	void SetButton(int button, bool state);
	void SetLastInputs();
	void SetMousePosition(int x, int y);
	sf::Vector2f GetMousePosition();
	Collider* GetMouseCollider();

private:
	//Saves current frames keypresses
	bool m_keys[255];
	//Saves last frames keypresses
	bool m_lastKeys[255];

	//Saves current frames buttonpresses
	bool m_buttons[3];
	//Saves last frames buttonpresses
	bool m_lastButtons[3];

	sf::Vector2f m_mouse_position;
	Collider* m_mouseCollider;
};