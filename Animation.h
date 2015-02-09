#pragma once

struct Frame
{
	float frameTime;
	sf::IntRect sourceRectangle;
};
class Animation
{
public:
	Animation(bool looping);

	void AddFrame(Frame* frame);
	Frame* GetFrame(int index);
	bool Looping();
	int Size();

private:
	bool m_looping;
	std::vector<Frame*> m_animations;
};