#pragma once

class Animation
{
public:
	Animation(sf::Sprite* sprite, std::string fileName);
	~Animation();

	void Update(float deltaTime);

private:
	float m_delay;
	sf::Sprite* m_sprite;
	float m_timer;
};	