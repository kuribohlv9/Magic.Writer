#include "stdafx.h"
#include <fstream>
#include "Animator.h"
#include "Animation.h"

Animator::Animator(sf::Sprite* sprite, std::string fileName)
{
	m_frame = 0;
	m_timer = 0;
	m_complete = false;
	m_sprite = sprite;

	std::ifstream stream;
	stream.open(fileName);

	//Load variables
	std::string animationName;
	int frameCount;
	float frameDelay;
	bool looping;
	sf::IntRect sourceRectangle;

	stream >> animationName;
	stream >> frameCount;
	stream >> looping;

	Animation* animation = new Animation(looping);
	
	while (!stream.eof())
	{
		//read sprite position from file
		stream >> frameDelay;
		stream >> sourceRectangle.left;
		stream >> sourceRectangle.top;
		stream >> sourceRectangle.width;
		stream >> sourceRectangle.height;

		Frame* frame = new Frame();
		frame->frameTime = frameDelay;
		frame->sourceRectangle = sourceRectangle;

		animation->AddFrame(frame);

		if (animation->Size() >= frameCount)
		{
			m_animations.insert(std::pair<std::string, Animation*>(animationName, animation));

			stream >> animationName;
			stream >> frameCount;
			stream >> looping;

			animation = new Animation(looping);
		}
	}
	stream.close();
}
Animator::~Animator()
{

}
void Animator::SetAnimation(const std::string& animationName)
{
	if (m_animationName != animationName)
	{
		m_complete = false;
		m_frame = 0;
		m_timer = 0;
		m_animationName = animationName;
		m_sprite->setTextureRect(m_animations[m_animationName]->GetFrame(m_frame)->sourceRectangle);
	}
}
void Animator::Update(float deltaTime)
{
	if (m_animationName == "throwing")
	{
		int rawr = 0;
	}
	if (m_complete)
		return;

	m_timer += deltaTime;

	Animation* anim = m_animations[m_animationName];

	if (m_timer >= anim->GetFrame(m_frame)->frameTime)
	{
		m_frame++;

		if (m_frame >= anim->Size())
		{
			if (anim->Looping())
			{
				m_frame = 0;
			}
			else
			{
				m_complete = true;
				return;
			}
		}

		m_sprite->setTextureRect(anim->GetFrame(m_frame)->sourceRectangle);
		
		//reset timer		
		m_timer = 0;
	}
}

bool Animator::Complete()
{
	return m_complete;
}