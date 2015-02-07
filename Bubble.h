#pragma once

class Item;
class DrawManager;

class Bubble
{
public:
	Bubble(float x, float y, sf::Texture* texture);

	void Draw(DrawManager* drawManager);

	void SetItem(Item* item);
	Item* GetItem();

private:
	int m_width;
	int m_height;
	sf::Sprite m_sprite;
	Item* m_item;
};