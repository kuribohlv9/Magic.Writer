#pragma once

class Particle;
class DrawManager;

class ParticleEmitter
{
public:
	ParticleEmitter(sf::Texture* texture, int particleCount);
	~ParticleEmitter();

	void Update(float deltaTime);
	void Draw(DrawManager* drawManager);

	void ActivateParticle();
	void SetPosition(float x, float y);
	void SetStartAngle(int angleMin, int angleMax);
	void SetAcceleration(float x, float y);
	void SetLifeTime(float secondsMin, float secondsMax);
	void SetSize(int width, int height);
	void SetActive(bool state);
	void SetSpawnRate(float spawnRate);
	void SetForce(int forceMin, int forceMax);
	bool IsActive();
private:
	//Holds all particles
	std::vector<Particle*> m_particles;

	//Contains an area which a particle is randomly created within
	sf::IntRect m_area;
	//What velocity to activate a particle in
	sf::Vector2f m_startDirection;
	//What acceleration to give an activated particle
	sf::Vector2f m_acceleration;
	//Stores the particles lifetime
	float m_secondsToLive;
	//A boolean if the particle should be scaled or not
	bool m_scaleParticle;
	//At what rate should we activate each particle (milliseconds)
	float m_spawnRate;
	//A timer used to activate particles
	float m_timer;

	bool m_active;
	float m_force;
};

