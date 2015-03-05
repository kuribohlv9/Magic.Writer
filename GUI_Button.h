#pragma once
#include "GUI_Element.h"

class GUI_Label;
class DrawManager;
class InputManager;
class Collider;

class GUI_Button : public GUI_Element
{
public:
	GUI_Button(float x, float y, GUI_Element* parent, sf::Texture* texture, sf::IntRect sourceRectangle);
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
	sf::IntRect m_buttonRectangle;
	bool m_hovered;
	bool m_pressed;
};