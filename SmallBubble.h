#pragma once

class DrawManager;
class Bubble;
class Player;

class SmallBubble
{
public:
	SmallBubble(sf::Texture* texture, Player* player, Bubble* targetBubble, float percentageFromPlayer);
	~SmallBubble();

	void Update(float deltaTime);
	void Draw(DrawManager* drawManager);
	bool IsActive();

private:
	sf::Sprite m_sprite;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	float m_acceleration;
	Player* m_player;
	Bubble* m_targetBubble;

	float m_percantageFromPlayer;
	bool m_active;
};

