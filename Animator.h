#pragma once

class Animation;

class Animator
{
public:
	Animator(sf::Sprite* sprite, std::string fileName);
	~Animator();

	void Update(float deltaTime);
	void SetAnimation(const std::string& AnimatorName);

	bool Complete();

private:
	float m_timer;
	int m_frame;
	bool m_complete;
	std::string m_animationName;
	sf::Sprite* m_sprite;
	std::map<std::string, Animation*> m_animations;
};	