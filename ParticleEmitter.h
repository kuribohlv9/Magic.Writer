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
	sf::IntRect m_area;

	std::vector<Particle*> m_particles;
	sf::Vector2f m_startVelocity;
	sf::Vector2f m_acceleration;
	sf::Texture* m_particleTexture;
	float m_secondsToLive;
	bool m_scaleParticle;
	float m_spawnRate;
	float m_timer;
};

