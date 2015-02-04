// GameObject.h
class Collider;
class DrawManager;

#pragma once

enum GType
{
	GAMEOBJECT_UNKNOWN,
	GAMEOBJECT_MONSTERS,
	GAMEOBJECT_PLAYER,
	GAMEOBJECT_ITEM
};

enum ItemProperties
{
	HARD,
	SOFT,
	ALIVE,
	DEAD
};

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Draw(DrawManager* drawManager);
	virtual void Update(float deltaTime);

	GType GetType();
	sf::Sprite* GetSprite();
	Collider* GetCollider();
	bool IsActive();
	float GetX();
	float GetY();
	void Move(float x, float y);
	void SetPosition(float x, float y);
	void SetActive(bool state);

protected:
	float m_x;
	float m_y;
	bool m_active;
	sf::Sprite m_sprite;
	Collider* m_collider;
	GType m_type;
};