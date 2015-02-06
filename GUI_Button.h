#pragma once
#include "GUI_Element.h"

class GUI_Label;
class DrawManager;
class InputManager;
class Collider;

class GUI_Button : public GUI_Element
{
public:
	GUI_Button(int x, int y, GUI_Element* parent, sf::Texture* texture, int buttonWidth, int buttonHeight);
	~GUI_Button();
	void Update();
	void Draw(DrawManager* drawManager);
	void SetParent(GUI_Element* parent);
	void Refresh();
	void SetLabel(GUI_Label* label);
	bool IsPressed();
	GUI_Label* GetLabel();
	sf::Sprite* GetSprite();

private:
	InputManager* m_inputManager;
	Collider* m_collider;
	GUI_Label* m_label;
	sf::Sprite m_sprite;
	bool m_hovered;
	bool m_pressed;
	int m_buttonHeight;
	int m_buttonWidth;
};