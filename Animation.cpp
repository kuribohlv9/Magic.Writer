#include "stdafx.h"
#include <fstream>
#include "Animation.h"

Animation::Animation(sf::Sprite* sprite, std::string fileName)
{
	m_sprite = sprite;
	std::ifstream stream;

	stream.open(fileName);

	sf::IntRect sourceRectangle;
	//Read frameDelay from file
	int frameDelay;
	int row = 0;

	stream >> frameDelay;
	
	//while (!stream.eof())
	//{
	//	//Read sprite position from file
	//	stream >> x;
	//	stream >> y;
	//	stream >> width;
	//	stream >> height;
	//}

	stream.close();

	m_timer = 0;
}

Animation::~Animation()
{

}

void Animation::Update(float deltaTime)
{
	m_timer += deltaTime;

	if (m_timer >= m_delay)
	{
		//next frame
		
		
		
		
		//reset timer		
		m_timer = 0;
	}
}