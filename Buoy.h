#pragma once

class Animator;
class DrawManager;
class Buoy
{
public:
	Buoy(sf::Texture* texture, float x, float y);
	~Buoy();

	void Update(float deltaTime);
	void Draw(DrawManager* drawManager);
	void SwitchFlag();

private:
	Animator* m_animator;
	sf::Sprite m_sprite;
};

