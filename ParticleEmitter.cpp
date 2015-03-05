#include "stdafx.h"
#include "ParticleEmitter.h"
#include "Particle.h"
#include  "DrawManager.h"

ParticleEmitter::ParticleEmitter(sf::Texture* texture, int particleCount)
{
	SetStartAngle(0, 1);
	SetForce(200, 200);
	m_acceleration = sf::Vector2f(0, 0);
	m_area = sf::IntRect(0, 0, 1, 1);
	m_spawnRate = 1;
	m_secondsToLive = 2;
	m_rotationVelocity = 0;
	m_scaleParticle = true;
	m_active = true;

	for (int i = 0; i < particleCount; i++)
	{
		Particle* p = new Particle(texture);
		m_particles.push_back(p);
	}
}
ParticleEmitter::~ParticleEmitter()
{
	auto it = m_particles.begin();

	while (it != m_particles.end())
	{
		delete *it;
		++it;
	}
}

void ParticleEmitter::Update(float deltaTime)
{
	//Increase timer
	m_timer += deltaTime;

	//Check for timer tick
	if (m_timer > m_spawnRate && m_active)
	{
		//Reset timer
		m_timer = 0;

		//Create a new particle
		ActivateParticle();
	}


	//Update active particles
	for (int i = 0; i < m_particles.size(); i++)
	{
		if (!m_particles[i]->Active())
			continue;

		m_particles[i]->Update(deltaTime);
	}
}
void ParticleEmitter::Draw(DrawManager* drawManager)
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		if (!m_particles[i]->Active())
			continue;

		m_particles[i]->Draw(drawManager);
	}
}

void ParticleEmitter::ActivateParticle()
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		if (!m_particles[i]->Active())
		{
			//Calculate a position
			sf::Vector2f pos = sf::Vector2f(m_area.left, m_area.top);
			pos.x += rand() % m_area.width;
			pos.y += rand() % m_area.height;


			//Activate particle
			m_particles[i]->Activate(
				pos,
				m_acceleration,
				m_startDirection,
				m_secondsToLive,
				m_scaleParticle,
				m_rotationVelocity);
			break;
		}
	}
}
void ParticleEmitter::SetPosition(float x, float y)
{
	m_area.left = x - m_area.width;
	m_area.top = y - m_area.height;
}
void ParticleEmitter::SetStartAngle(int angleMin, int angleMax)
{
	//Randomize a angle
	if (angleMin == angleMax)
		angleMax++;

	int angle = angleMin + rand() % (angleMax - angleMin);
	float radians = angle * (3.14 / 180);
	//Calculate direction from angle
	sf::Vector2f dir = sf::Vector2f(cos(radians), sin(radians));

	//Normalize direction
	float length = sqrt(dir.x * dir.x + dir.y * dir.y);
	dir /= length;

	dir *= m_force;

	//Apply start direction
	m_startDirection = dir;
}
void ParticleEmitter::SetForce(int forceMin, int forceMax)
{
	if (forceMin == forceMax)
		forceMax++;
	m_force = forceMin + rand() % (forceMax - forceMin);
}
void ParticleEmitter::SetAcceleration(float x, float y)
{
	m_acceleration = sf::Vector2f(x, y);
}
void ParticleEmitter::SetLifeTime(float secondsMin, float secondsMax)
{
	int max = secondsMax * 10;
	int rndInt = rand() % max;
	float rnd = rndInt / 10;
	m_secondsToLive = secondsMin + rnd;
}
void ParticleEmitter::SetSize(int width, int height)
{
	m_area.width = width;
	m_area.height = height;
}
void ParticleEmitter::SetActive(bool state)
{
	m_active = state;
}
void ParticleEmitter::SetSpawnRate(float spawnRate)
{
	m_spawnRate = spawnRate;
}
void ParticleEmitter::SetRotationVelocity(float velocity)
{
	m_rotationVelocity = velocity;
}
bool ParticleEmitter::IsActive()
{
	return m_active;
}
