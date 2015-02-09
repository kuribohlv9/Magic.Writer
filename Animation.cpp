#include "stdafx.h"
#include "Animation.h"

Animation::Animation(bool looping)
{
	m_looping = looping;
}

void Animation::AddFrame(Frame* frame)
{
	m_animations.push_back(frame);
}
Frame* Animation::GetFrame(int index)
{
	return m_animations[index];
}
bool Animation::Looping()
{
	return m_looping;
}
int Animation::Size()
{
	return m_animations.size();
}