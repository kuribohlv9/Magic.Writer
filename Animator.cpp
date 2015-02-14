#include "stdafx.h"
#include <fstream>
#include "Animator.h"
#include "Animation.h"

Animator::Animator(sf::Sprite* sprite, const std::string& animationTextFileName)
{
	//Set starting values for variables
	m_frame = 0;
	m_timer = 0;
	m_complete = false;
	m_sprite = sprite;

	//Load animations from .txt file
	LoadAnimations(animationTextFileName);
}
Animator::~Animator()
{
	//Delete all newed animations
	auto it = m_animations.begin();
	while (it != m_animations.end())
	{
		delete (it->second);
		++it;
	}

	m_animations.clear();
}

void Animator::SetAnimation(const std::string& animationName)
{
	//Set a new animation
	if (m_animationName != animationName)
	{
		//Reset values
		m_complete = false;
		m_frame = 0;
		m_timer = 0;
		m_animationName = animationName;
		m_sprite->setTextureRect(m_animations[m_animationName]->GetFrame(m_frame)->sourceRectangle);
	}
}
void Animator::Update(float deltaTime)
{
	//Return if the animation is complete
	if (m_complete)
		return;

	//Increase timer
	m_timer += deltaTime;

	//Get the active animation
	Animation* anim = m_animations[m_animationName];

	//Check if the timer has reached the current frames visible time
	if (m_timer >= anim->GetFrame(m_frame)->frameTime)
	{
		//Increase the frame counter
		m_frame++;

		//Check if the frame count has reached beyond the animations framecount
		if (m_frame >= anim->Size())
		{
			//If the animation is loopable, reset the framecount to 0
			if (anim->Looping())
			{
				m_frame = 0;
			}
			else //If not loopable, set the animation to be complete and return.
			{
				m_complete = true;
				return;
			}
		}

		//Set the sprites source rectangle to the current frames source rectangle
		m_sprite->setTextureRect(anim->GetFrame(m_frame)->sourceRectangle);
		
		//Reset timer		
		m_timer = 0;
	}
}

bool Animator::Complete()
{
	//Return if the animation is complete or not
	return m_complete;
}
void Animator::LoadAnimations(const std::string& filename)
{
	//Open a file stream
	std::ifstream stream;
	stream.open(filename);

	//Create variables for loading animation
	std::string animationName;
	int frameCount;
	bool looping;

	//Load the first three variables for the animation
	stream >> animationName;
	stream >> frameCount;
	stream >> looping;

	//Create a new animation
	Animation* animation = new Animation(looping);


	//Start a loop for reading each frame in the .txt file
	while (!stream.eof())
	{
		//Create a new frame
		Frame frame;

		//Read the frames visible time
		stream >> frame.frameTime;

		//Read source rectangles values
		stream >> frame.sourceRectangle.left;
		stream >> frame.sourceRectangle.top;
		stream >> frame.sourceRectangle.width;
		stream >> frame.sourceRectangle.height;

		//Add the frame to the animation
		animation->AddFrame(frame);

		//Check to see if the animations framecount is correct
		if (animation->Size() >= frameCount)
		{
			//Add the complete animation to a map with it's name as the key
			m_animations.insert(std::pair<std::string, Animation*>(animationName, animation));

			//Load the next animations values
			stream >> animationName;
			stream >> frameCount;
			stream >> looping;

			//Create a new animation
			animation = new Animation(looping);
		}
	}
	//Close the file stream
	stream.close();
}