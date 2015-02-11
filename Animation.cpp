#include "stdafx.h"
#include "Animation.h"

Animation::Animation(bool looping)
{
	m_looping = looping;
}

Animation::~Animation()
{
}

void Animation::AddFrame(Frame frame)
{
	m_frames.push_back(frame);
}
Frame* Animation::GetFrame(const int index)
{
	if (index < Size())
		return &m_frames[index];
	else
		return nullptr;
}
bool Animation::Looping()
{
	return m_looping;
}
int Animation::Size()
{
	return m_frames.size();
}