#pragma once

class DrawManager;

class Particle
{
public:
	Particle(sf::Texture* texture);
	~Particle();

	void Update(float deltaTime);
	void Draw(DrawManager* drawManager);

	bool Active();
	void Activate(sf::Vector2f position, sf::Vector2f acceleration, sf::Vector2f velocity, float secondsToLive, bool scaleParticle);

private:
	//Stores the current speed of which the particle is moving
	sf::Vector2f m_velocity;
	//Acceleration will be added to the velocity every frame
	sf::Vector2f m_acceleration;
	//Stores the particles sprite
	sf::Sprite m_sprite;
	//The lifetime of the particle (How long will it be showed on the screen)
	float m_secondsToLive;
	//The current life of the particle, goes from 255 to 0
	int m_life;
	//A timer used to decrease life and change particle in other ways
	float m_timer;
	//A boolean if the particle is currently active or not
	bool m_active;
	//A boolean if the particle should scale itself or not
	bool m_scaleParticle;
};

