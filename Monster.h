#pragma once
#include "GameObject.h"

class Animator;
class DrawManager;
class ParticleEmitter;

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
	Monster(sf::Texture* texture, const std::string& animationFile, int spriteWidth, int spriteHeight, float speed, ItemProperty weakness, sf::Texture* particleTexture);
	~Monster();

	void Draw(DrawManager* drawManager);
	void Update(float deltaTime);
	void Freeze(bool state);
	void Damage(ItemProperty property, int &score);
	void Activate();
	bool Burst();

private:
	void HandleBodyParts();

private:
	MonsterState m_state;
	ItemProperty m_weakness;

	ParticleEmitter* m_emitter;
	Animator* m_head_animator;
	sf::Sprite m_head_sprite;
	sf::Sprite m_snail_sprite;
	sf::Sprite m_foam_sprite;
	int m_sprite_width;
	int m_sprite_height;

	float m_totalLifeTime;
	bool m_frozen;
	bool m_activeBurst;
	int m_health;
	float m_speed;
	float m_originalSpeed;
};