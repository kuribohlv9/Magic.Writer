#pragma once
#include "GameObject.h"

class Animator;
class DrawManager;
class InputManager;
class Item;
class ParticleEmitter;

enum PlayerState
{
	PLAYER_IDLE,
	PLAYER_CHANTING,
	PLAYER_HOLDING,
	PLAYER_THROWING,
	PLAYER_KNOCKEDDOWN,
	PLAYER_JUMPING
};

class Player : public GameObject
{
public:
	Player(sf::Texture* texture, sf::Texture* particle, sf::SoundBuffer* changeLaneBuffer);
	~Player();
	void Update(float deltaTime);
	void Draw(DrawManager* drawManager);

	void SetItem(Item* item);
	Item* GetItem();

	void Knockdown();
	bool IsStunned();
	int GetCurrentLane();
	void ChantingAnimation();

private:
	void HandleMovement();
	void ChangeLane(int xDirection);

private:
	ParticleEmitter* m_emitter;
	Animator* m_animator;
	InputManager* m_inputManager;
	Item* m_item;
	PlayerState m_state;
	sf::Sound m_changeLaneSound;
	int m_lane;
};