#pragma once

class Animation;

class Animator
{
public:
	Animator(sf::Sprite* sprite, const std::string& animationTextFileName);
	~Animator();

	void Update(float deltaTime);
	void SetAnimation(const std::string& animationName);
	void SetStartFrame(int frame);

	bool Complete();

private:
	void LoadAnimations(const std::string& filename);

private:
	float m_timer;
	int m_frame;
	bool m_complete;
	std::string m_animationName;
	sf::Sprite* m_sprite;
	std::map<std::string, Animation*> m_animations;
};