#pragma once

class Item;
class DrawManager;
class Player;

class Bubble
{
public:
	Bubble(float x, float y, sf::Texture* texture, Player* player);

	void Draw(DrawManager* drawManager);
	void Update(float deltaTime);

	void SetItem(Item* item);
	Item* GetItem();
	sf::Vector2f GetPosition();

private:
	float m_speed;
	int m_playerOffset;
	int m_width;
	int m_height;
	sf::Sprite m_sprite;
	Item* m_item;
	Player* m_player;
};