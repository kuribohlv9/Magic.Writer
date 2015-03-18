#pragma once
#include "GameObject.h"

enum ItemState
{
	ITEM_HOLDING,
	ITEM_FLYING,
	ITEM_HIT
};

class ParticleEmitter;
class DrawManager;
class Item : public GameObject
{
public:
	Item(sf::Texture* texture, sf::Texture* particle, ItemProperty property, const std::string& name);
	~Item();

	void Update(float deltaTime);
	void Draw(DrawManager* drawManager);

	std::string GetName();
	ItemProperty GetProperty();

	bool IsInGame();
	void SetInGame(bool state);
	void SetSourceRectangle(int index, sf::IntRect rectangle);
	void SetName(const std::string& name);

	void Activate();
	void Reset();
	void SetState(ItemState state);
	ItemState GetState();
	sf::Vector2f GetItemBounceDir();

private:
	ItemState m_state;
	std::string m_name;
	ItemProperty m_property;
	sf::IntRect m_sourceRectangles[3];
	ParticleEmitter* m_emitter;

	bool m_inGame;
	float m_speed;
	sf::Vector2f m_bounceDir;
	bool m_bounceAway;
};