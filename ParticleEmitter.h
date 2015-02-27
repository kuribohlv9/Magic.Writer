#pragma once

class Particle;
class DrawManager;

class ParticleEmitter
{
public:
	ParticleEmitter(int particleCount, float spawnRate, sf::Texture* particleTexture);
	~ParticleEmitter();

	void Update(float deltaTime);
	void Draw(DrawManager* drawManager);


	void CreateParticle();
	void SetPosition(sf::Vector2f position);
	void SetStartVelocity(sf::Vector2f velocity);
	void SetAcceleration(sf::Vector2f acceleration);
	void SetLifeTime(float secondsToLive);
	void SetSize(int width, int height);
	void ScaleParticle(bool state);

private:
	//Contains an area which a particle is randomly created within
	sf::IntRect m_area;

	//Stores all the created particles
	std::vector<Particle*> m_particles;
	//What velocity to activate a particle in
	sf::Vector2f m_startVelocity;
	//What acceleration to give an activated particle
	sf::Vector2f m_acceleration;
	//What texture the particle should have
	sf::Texture* m_particleTexture;
	//Stores the particles lifetime
	float m_secondsToLive;
	//A boolean if the particle should be scaled or not
	bool m_scaleParticle;
	//At what rate should we activate each particle (milliseconds)
	float m_spawnRate;
	//A timer used to activate particles
	float m_timer;
};

