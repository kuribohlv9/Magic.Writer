#pragma once

class Particle;
class ParticleEmitter;
class DrawManager;

class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	ParticleEmitter* CreateEmitter(sf::Texture* texture, int particleCount);

	void Update(float deltaTime);
	void Draw(DrawManager* drawManager);

private:
	std::vector<ParticleEmitter*> m_emitters;
};