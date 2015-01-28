// GameObject.h
class Collider;

#pragma once

enum GType
{
	ENTITY_UNKNOWN,
	ENTITY_MONSTERS,

};

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	// Inside Draw we draw out the sprite using sf::RenderWindow.
	virtual void Draw(sf::RenderWindow* window);
	virtual void Update(float deltaTime);
	virtual GType GetType();
	virtual sf::Sprite* GetSprite();
	virtual Collider* GetCollider();
	virtual bool IsActive();
	virtual float GetX();
	virtual float GetY();

protected:
	float m_x;
	float m_y;
	bool m_active;
	sf::Sprite m_sprite;
	Collider* m_collider;
	GType m_type;
};