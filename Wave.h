#pragma once
#include "GameObject.h"

class Animator;
class DrawManager;

class Wave : public GameObject
{
public:
	Wave(sf::Texture* texture);
	~Wave();
	
	void Update(float deltaTime);
	void Draw(DrawManager* drawManager);

	void Activate();

private:
	Animator* m_animator;
	float m_speed;

};

