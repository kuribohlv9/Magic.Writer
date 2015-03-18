#pragma once
#include "State.h"

class AudioManager;
class ItemManager;
class WordManager;
class DrawManager;
class TextureManager;
class InputManager;
class WaveManager;
class PowerManager;
class ParticleEmitter;
class ParticleManager;
class HighscoreManager;
class BubbleManager;
class Item;
class Monster;
class Player;
class Wave;
class GUI_Button;
class GUI_Label;

enum mode {
	MODE_UNKNOWN,
	MODE_PLAYING,
	MODE_DEFEAT,
	MODE_VICTORY
};

struct UserInfo
{	
	//Scores
	int neutralHitScore = 50;
	int criticalHitScore = 150;
	int monsterDefeatedScore = 100;

	int monstersDefeated;
	int oneShots;
	int totalScore;
};

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
	void SetUserInfo();

	void InstantiateBubbles();
	void InstantiateMonsters();
	void SpawnMonster();
	
	bool PlayMode(float deltaTime);
	bool VictoryMode(float deltaTime);
	bool DefeatMode(float deltaTime);

	bool IsMonsters();

private:
	//Test
	mode m_status;
	ScreenState m_next_state;
	int m_wave_level;

	//Manager
	DrawManager* m_drawManager;
	TextureManager* m_textureManager;
	InputManager* m_inputManager;
	AudioManager* m_audioManager;
	WordManager* m_wordManager;
	ItemManager* m_itemManager;
	WaveManager* m_waveManager;
	PowerManager* m_powerManager;
	ParticleManager* m_particleManager;
	HighscoreManager* m_highscoreManager;
	BubbleManager* m_bubbleManager;

	//Items
	std::vector<Item*> m_activeItems;

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
	sf::Sound m_conjureCompleteSound;

	//test font
	sf::Font* m_font;

	//Test HUD
	sf::Sprite m_lifeSprite, m_lifeSprite2, m_lifeSprite3;
	sf::Text m_scoreDisplay;
	int m_score;
	int m_lastScore;
	unsigned int m_life;

	float m_speed;

	//Victory and Losing screens
	sf::Sprite m_victory_window;
	sf::Sprite m_defeat_window;
	GUI_Button* m_back_to_menu_button;
	GUI_Button* m_next_wave_button;
	sf::Text m_userInfoText;
	UserInfo m_userInfo;

	//Highscore input
	GUI_Button* m_submit_button;
	sf::Text m_userTextBox;
	std::string m_userName;

	//Music
	std::vector<sf::Music*> m_game_themes;
	sf::Music* m_active_theme;
	sf::Music* m_losing_theme;
};