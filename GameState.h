#pragma once
#include "State.h"

class AudioManager;
class ItemManager;
class WordManager;
class DrawManager;
class TextureManager;
class InputManager;
class WaveManager;
class PowerUpManager;
class ParticleEmitter;

class Item;
class Bubble;
class Monster;
class Player;
class Wave;

class GameState : public State
{
public:
	GameState();
	~GameState();

	bool Update(float deltaTime);
	void Draw();

	void Enter();
	void Exit();

	ScreenState NextState();

private:
	void ConvertWordToItem();
	void CheckCollision();
	void SpawnMonster();

private:
	//Manager
	DrawManager* m_drawManager;
	TextureManager* m_textureManager;
	InputManager* m_inputManager;
	AudioManager* m_audioManager;

	WordManager* m_wordManager;
	ItemManager* m_itemManager;
	WaveManager* m_waveManager;
	PowerUpManager* m_powerUpManager;

	//Item & word variables
	std::vector<Bubble*> m_bubbles;
	std::vector<Item*> m_activeItems;

	//Freeze
	bool m_frozen;
	float m_freezeTimer;

	//Monster
	std::vector<Monster*> m_monsters;

	//Waves
	std::vector<Wave*> m_waves;
	float m_waveTimer;

	//Background
	sf::Sprite m_backgroundSprite;
	sf::Sprite m_ice_backgroundSprite;

	//Player
	Player* m_player;

	//test font
	sf::Font* m_font;

	//Test HUD
	sf::Sprite m_life_sprite;
	sf::Text m_scoreDisplay;
	int m_score;
	int m_lastScore;
	unsigned int m_life;
};