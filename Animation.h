#pragma once

class Animation
{
public:
	Animation(sf::Sprite* sprite, std::string fileName);
	~Animation();

	void Update(float deltaTime);

private:
	sf::Sprite* m_sprite;
	float m_timer;
	std::vector<std::pair<float, sf::IntRect>> m_frames;
	int m_currentFrame;
};	