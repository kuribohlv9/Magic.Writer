#include "stdafx.h"
#include "Particle.h"
#include "DrawManager.h"


Particle::Particle(sf::Texture* texture)
{
	m_velocity = sf::Vector2f(0, 0);
	m_acceleration = sf::Vector2f(0, 0);
	m_sprite.setTexture(*texture);
	m_sprite.setOrigin(texture->getSize().x / 2.0f, texture->getSize().y / 2.0f);
	m_secondsToLive = 0;
	m_active = false;
}
Particle::~Particle()
{
}

void Particle::Update(float deltaTime)
{
	//Move particle
	m_velocity += m_acceleration;
	m_sprite.move(m_velocity * deltaTime);


	//Rotate particle
	m_sprite.rotate(m_rotationVelocity);

	m_timer += deltaTime;
	if (m_timer >= 1 / 60)
	{
		//Reset timer
		m_timer = 0;

		//Decrease life
		m_life -= (int)((255 / m_secondsToLive) / 60);

		//Fade particle to 0 alpha
		sf::Color c = m_sprite.getColor();
		c.a = m_life;
		m_sprite.setColor(c);

		//Scale particle to size 0
		if (m_scaleParticle)
		{
			float scale = (float)(m_life) / 255.0f;
			m_sprite.setScale(scale, scale);
		}

		//Deactivate if particle is dead
		m_active = m_life > 0;
	}
}
void Particle::Draw(DrawManager* drawManager)
{
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
}

bool Particle::Active()
{
	return m_active;
}
void Particle::Activate(sf::Vector2f position, sf::Vector2f acceleration, sf::Vector2f velocity, float secondsToLive, bool scaleParticle, float rotationVelocity)
{
	m_sprite.setPosition(position);
	m_acceleration = acceleration;
	m_velocity = velocity;
	m_secondsToLive = secondsToLive;
	m_life = 255;
	m_timer = 0;
	m_sprite.setColor(sf::Color::White);
	m_sprite.setScale(1, 1);
	m_active = true;
	m_scaleParticle = scaleParticle;

	m_rotationVelocity = rotationVelocity;
	if (m_rotationVelocity != 0)
	{
		m_sprite.setRotation(static_cast<float>(rand() % 360));
	}
}
