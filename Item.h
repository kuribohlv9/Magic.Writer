#pragma once
#include "GameObject.h"

class Item : public GameObject
{
public:
	Item(sf::Texture* texture, sf::IntRect sourceRectangle, ItemProperty property, const std::string& name);

	void Update(float deltaTime);

	std::string GetName();
	ItemProperty GetProperty();

	bool IsInGame();
	void SetInGame(bool state);
	void ActivateItem();

	void Reset();

private:
	std::string m_name;
	ItemProperty m_property;

	bool m_inGame;
	float m_speed;
	float m_rotation;
};