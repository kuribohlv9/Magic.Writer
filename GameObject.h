// GameObject.h
class Collider;
class DrawManager;

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

	// Inside Draw we draw out the sprite using drawManager
	virtual void Draw(DrawManager* drawManager);
	virtual void Update(float deltaTime);
	virtual GType GetType();
	virtual sf::Sprite* GetSprite();
	virtual Collider* GetCollider();
	virtual bool IsActive();
	virtual float GetX();
	virtual float GetY();
	virtual void Move(float x, float y);
	virtual void SetPosition(float x, float y);

protected:
	float m_x;
	float m_y;
	bool m_active;
	sf::Sprite m_sprite;
	Collider* m_collider;
	GType m_type;
};