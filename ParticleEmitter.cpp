#include "stdafx.h"
#include "ParticleEmitter.h"
#include "Particle.h"
#include  "DrawManager.h"

#include <iostream>

ParticleEmitter::ParticleEmitter(int particleCount, float spawnRate, sf::Texture* particleTexture)
{
	m_spawnRate = spawnRate;
	m_startVelocity = sf::Vector2f(0, -500);
	m_acceleration = sf::Vector2f(0, -5);
	m_area = sf::IntRect(0, 0, 1, 1);
	m_secondsToLive = 4;
	m_particleTexture = particleTexture;
	m_scaleParticle = false;

	for (int i = 0; i < particleCount; i++)
	{
		Particle* p = new Particle(m_particleTexture, m_secondsToLive);
		m_particles.push_back(p);
	}
}
ParticleEmitter::~ParticleEmitter()
{
	auto it = m_particles.begin();

	while (it != m_particles.end())
	{
		delete (*it);
		++it;
	}
	m_particles.clear();
}

void ParticleEmitter::Update(float deltaTime)
{
	//Increase timer
	m_timer += deltaTime;

	//Check for timer tick
	if (m_timer > m_spawnRate)
	{
		//Reset timer
		m_timer = 0;

		//Create a new particle
		CreateParticle();
	}

	//Update all active particles
	for (int i = 0; i < m_particles.size(); i++)
	{
		if (!m_particles[i]->Active())
			continue;

		m_particles[i]->Update(deltaTime);
	}
}
void ParticleEmitter::Draw(DrawManager* drawManager)
{
	//Draw all active particles
	for (int i = 0; i < m_particles.size(); i++)
	{
		if (!m_particles[i]->Active())
			continue;

		m_particles[i]->Draw(drawManager);
	}
}

void ParticleEmitter::CreateParticle()
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		if (!m_particles[i]->Active())
		{
			sf::Vector2f pos = sf::Vector2f(m_area.left, m_area.top);
			pos.x += rand() % m_area.width;
			pos.y += rand() % m_area.height;
			m_particles[i]->Activate(pos, m_acceleration, m_startVelocity, m_secondsToLive, m_scaleParticle);
			break;
		}
	}
}
void ParticleEmitter::SetPosition(sf::Vector2f position)
{
	m_area.left = position.x - m_area.width;
	m_area.top = position.y - m_area.height;
}
void ParticleEmitter::SetStartVelocity(sf::Vector2f velocity)
{
	m_startVelocity = velocity;
}
void ParticleEmitter::SetAcceleration(sf::Vector2f acceleration)
{
	m_acceleration = acceleration;
}
void ParticleEmitter::SetLifeTime(float secondsToLive)
{
	m_secondsToLive = secondsToLive;
}
void ParticleEmitter::SetSize(int width, int height)
{
	m_area.width = width;
	m_area.height = height;
}
void ParticleEmitter::ScaleParticle(bool state)
{
	m_scaleParticle = state;
}
