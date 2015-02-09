#include "stdafx.h"
#include <fstream>
#include "Animation.h"

Animation::Animation(sf::Sprite* sprite, std::string fileName)
{
	m_currentFrame = 0;
	m_sprite = sprite;
	m_timer = 0;

	std::ifstream stream;
	stream.open(fileName);

	sf::IntRect sourceRectangle;
	//Read frameDelay from file
	int frameDelay;
	
	while (!stream.eof())
	{
		//read sprite position from file
		stream >> frameDelay;
		stream >> sourceRectangle.left;
		stream >> sourceRectangle.top;
		stream >> sourceRectangle.width;
		stream >> sourceRectangle.height;

		m_frames.push_back(std::pair<float, sf::IntRect>(frameDelay, sourceRectangle));
	}
	stream.close();

	m_sprite->setTextureRect(m_frames[0].second);
}

Animation::~Animation()
{

}

void Animation::Update(float deltaTime)
{
	m_timer += deltaTime;
	int frameCount = sizeof(m_frames) / sizeof(m_frames[0]);

	if (m_timer >= m_frames[m_currentFrame].first)
	{
		m_currentFrame++;

		if (m_currentFrame >= frameCount)
		{
			m_currentFrame = 0;
		}

		m_sprite->setTextureRect(m_frames[m_currentFrame].second);
		
		//reset timer		
		m_timer = 0;
	}
}