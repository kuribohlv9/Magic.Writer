#include "stdafx.h"
#include "Particle.h"
#include "DrawManager.h"


Particle::Particle(sf::Texture* texture, float secondsToLive)
{
	m_velocity = sf::Vector2f(0, 0);
	m_acceleration = sf::Vector2f(0, 0);
	m_sprite.setTexture(*texture);
	m_sprite.setOrigin(texture->getSize().x / 2, texture->getSize().y / 2);
	m_secondsToLive = secondsToLive;
	m_active = false;
}
Particle::~Particle()
{
}

void Particle::Update(float deltaTime)
{
	m_velocity += m_acceleration;
	m_sprite.move(m_velocity * deltaTime);

	m_timer += deltaTime;
	if (m_timer >= 1 / 60)
	{
		m_timer = 0;
		m_life -= (255 / m_secondsToLive) / 60;

		sf::Color c = m_sprite.getColor();
		c.a = m_life;
		m_sprite.setColor(c);

		if (m_scaleParticle)
		{
			float scale = (float)(m_life) / 255.0f;
			m_sprite.setScale(scale, scale);
		}

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
void Particle::Activate(sf::Vector2f position, sf::Vector2f acceleration, sf::Vector2f startVelocity, float secondsToLive, bool scaleParticle)
{
	m_sprite.setPosition(position);
	m_acceleration = acceleration;
	m_velocity = startVelocity;
	m_secondsToLive = secondsToLive;
	m_life = 255;
	m_timer = 0;
	m_sprite.setColor(sf::Color::White);
	m_sprite.setScale(1, 1);
	m_active = true;
}
