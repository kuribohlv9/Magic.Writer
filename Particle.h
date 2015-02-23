#pragma once

class DrawManager;

class Particle
{
public:
	Particle(sf::Texture* texture, float secondsToLive);
	~Particle();

	void Update(float deltaTime);
	void Draw(DrawManager* drawManager);

	bool Active();
	void Activate(sf::Vector2f position, sf::Vector2f acceleration, sf::Vector2f startVelocity, float secondsToLive, bool scaleParticle);

private:
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
	sf::Sprite m_sprite;
	float m_secondsToLive;
	int m_life;
	float m_timer;
	bool m_active;
	bool m_scaleParticle;
};

