#pragma once
#include "GUI_Element.h"

class GUI_Panel : public GUI_Element
{
public:
	GUI_Panel(int x, int y, GUI_Element* parent, sf::Texture* texture);
	~GUI_Panel();
	void Update();
	void Draw(DrawManager* drawManager);

	void AddChild(GUI_Element* child);
private:
	std::vector<GUI_Element*> m_children;
	sf::Sprite m_sprite;
};