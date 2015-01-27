// GameObjects.h

#pragma once

enum GType
{
	ENTITY_UNKNOWN,
	ENTITY_MONSTERS,

};

class GameObjects
{
public:
	GameObjects();
	virtual ~GameObjects();

protected:
	// Inside Draw we draw out the sprite using sf::RenderWindow.
	virtual void Draw(sf::RenderWindow window) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual GType GetType();
	virtual sf::Sprite* GetSprite();
	// We were not in need of a collider class as it was a built-in class in SFML, so we just used sf::IntRect.
	virtual sf::IntRect* GetHitBox();
	virtual bool IsActive();
	virtual float GetX();
	virtual float GetY();

protected:
	float m_x;
	float m_y;
	bool m_active;
	sf::Sprite* m_sprite;
	sf::IntRect* m_hitBox;
	GType m_type;
};