#pragma once
#include "GUI_Element.h"

class DrawManager;

class GUI_Label : public GUI_Element
{
public:
	GUI_Label(float x, float y, GUI_Element* parent, sf::Font* font, std::string text);

	sf::Text* GetText();
	void Draw(DrawManager* drawManager);
	void Refresh();
	void SetString(const std::string& text);
	void SetColor(const sf::Color& color);
private:
	sf::Text m_text;
};