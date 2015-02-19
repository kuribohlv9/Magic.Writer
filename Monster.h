#pragma once
#include "GameObject.h"

class Animator;
class DrawManager;

enum MonsterState
{
	MONSTER_MOVE,
	MONSTER_HIT,
	MONSTER_CRITICAL,
	MONSTER_DEATH
};

class Monster : public GameObject
{
public:
	Monster(sf::Texture* texture, const std::string& animationFile, int spriteWidth, int spriteHeight, float speed, ItemProperty weakness);
	~Monster();

	void Draw(DrawManager* drawManager);
	void Update(float deltaTime);
	void Freeze(float time);
	void Damage(ItemProperty property, int &score);
	void Activate();

private:
	MonsterState m_state;
	ItemProperty m_weakness;

	Animator* m_head_animator;
	sf::Sprite m_head_sprite;
	sf::Sprite m_snail_sprite;
	sf::Sprite m_foam_sprite;
	int m_sprite_width;
	int m_sprite_height;

	float m_totalLifeTime;
	bool m_frozen;
	int m_health;
	float m_speed;
	float m_originalSpeed;
	float m_freezeTimer;
	float m_unfreezeDelay;
};