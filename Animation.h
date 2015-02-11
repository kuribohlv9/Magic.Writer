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
	~Animation();

	void AddFrame(Frame frame);
	Frame* GetFrame(const int index);
	bool Looping();
	int Size();

private:
	bool m_looping;
	std::vector<Frame> m_frames;
};