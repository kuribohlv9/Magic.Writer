#include "stdafx.h"
#include "ParticleManager.h"
#include "Particle.h"
#include "ParticleEmitter.h"

ParticleManager::ParticleManager()
{

}
ParticleManager::~ParticleManager()
{
	auto it = m_emitters.begin();

	while (it != m_emitters.end())
	{
		delete *it;
		++it;
	}
	m_emitters.clear();
}

ParticleEmitter* ParticleManager::CreateEmitter(sf::Texture* texture, int particleCount)
{
	ParticleEmitter* emitter = new ParticleEmitter(texture, particleCount);
	m_emitters.push_back(emitter);

	return emitter;
}

void ParticleManager::Update(float deltaTime)
{
	for (unsigned int i = 0; i < m_emitters.size(); i++)
	{
		m_emitters[i]->Update(deltaTime);
	}
}
void ParticleManager::Draw(DrawManager* drawManager)
{
	for (unsigned int i = 0; i < m_emitters.size(); i++)
	{
		m_emitters[i]->Draw(drawManager);
	}
}
void ParticleManager::Reset()
{
	auto it = m_emitters.begin();
	while (it != m_emitters.end())
	{
		delete *it;
		*it = nullptr;
		++it;
	}
	m_emitters.clear();
}