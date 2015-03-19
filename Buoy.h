#pragma once

class Animator;
class DrawManager;
class Buoy
{
public:
	Buoy(sf::Texture* texture, float x, float y, const std::string animationFile);
	~Buoy();

	void Update(float deltaTime);
	void Draw(DrawManager* drawManager);
	void SwitchFlag();

private:
	Animator* m_animator;
	sf::Sprite m_sprite;
	float m_height;
	bool m_red;
};

