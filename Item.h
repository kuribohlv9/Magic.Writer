#pragma once
#include "GameObject.h"

class Item : public GameObject
{
public:
	Item(sf::Texture* texture, sf::IntRect sourceRectangle, ItemProperties propertyOne, ItemProperties propertyTwo, const std::string& name);

	void Update(float deltaTime);

	std::string GetName();

private:
	std::string m_name;
	ItemProperties m_propertyOne;
	ItemProperties m_propertyTwo;

	float m_speed;
	float m_rotation;
};

