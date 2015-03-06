#include "stdafx.h"
#include "GameState.h"
#include <time.h>

//Manager
#include "ServiceLocator.h"
#include "AudioManager.h"
#include "DrawManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "CollisionManager.h"
#include "WordManager.h"
#include "ItemManager.h"
#include "WaveManager.h"
#include "PowerUpManager.h"
#include "ParticleManager.h"

//Classes
#include "Monster.h"
#include "Collider.h"
#include "Item.h"
#include "Player.h"
#include "Bubble.h"
#include "Utility.h"
#include "Wave.h"
#include "ParticleEmitter.h"
#include "GUI_Button.h"


GameState::GameState()
{
	srand(time(NULL));

	//Initiallize managers
	m_textureManager = ServiceLocator<TextureManager>::GetService();
	m_drawManager = ServiceLocator<DrawManager>::GetService();
	m_inputManager = ServiceLocator<InputManager >::GetService();
	m_audioManager = ServiceLocator<AudioManager>::GetService();
	m_wordManager = new WordManager();
	m_itemManager = new ItemManager();
	m_waveManager = new WaveManager();
	m_particleManager = ServiceLocator<ParticleManager>::GetService();

	//Load background texture
	sf::Texture* texture = m_textureManager->LoadTexture("assets/sprites/background/background.png");
	m_backgroundSprite.setTexture(*texture);
	texture = m_textureManager->LoadTexture("assets/sprites/background/ice_background.png");
	m_ice_backgroundSprite.setTexture(*texture);

	//Instantiate player
	sf::Texture* particleTexture = m_textureManager->LoadTexture("assets/sprites/wizard/particle.png");
	texture = m_textureManager->LoadTexture("assets/sprites/wizard/wizard_spritesheet.png");
	sf::SoundBuffer* buffer = m_audioManager->LoadSoundFromFile("assets/audio/complete/Wizard_walk_sound.wav");
	m_player = new Player(texture, particleTexture, buffer);

	m_powerUpManager = new PowerUpManager(&m_monsters, &m_activeItems, m_player);

	//Load sound
	buffer = m_audioManager->LoadSoundFromFile("assets/audio/complete/Wizard_spell_complete01.wav");
	m_conjureCompleteSound.setBuffer(*buffer);
	m_conjureCompleteSound.setVolume(7);

	InstantiateBubbles();
	InstantiateMonsters();
	
	//TEMPORARY HUD
	m_font = m_textureManager->LoadFont("assets/fonts/font.ttf");
	m_score_sign_sprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/sign_score.png"));
	m_score_sign_sprite.setPosition(ScreenWidth - 400, 0);
	m_life_sprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/HUD/life.png"));
	
	m_scoreDisplay.setFont(*m_font);
	m_scoreDisplay.setCharacterSize(40);
	m_scoreDisplay.setPosition(1620, 90);
	m_scoreDisplay.setString("0");
	m_scoreDisplay.setColor(sf::Color(0, 28, 34, 255));

	m_score = 0;
	m_lastScore = 0;
	m_life = 3;
	m_speed = 800;
	m_wave_level = 1;

	//Instantiate waves
	texture = m_textureManager->LoadTexture("assets/sprites/wave_spritesheet.png");

	//Wave pool
	for (int i = 0; i < 5; i++)
	{
		Wave* wave = new Wave(texture);
		m_waves.push_back(wave);
	}

	//Victory and Losing screen
	m_victory_window.setPosition(300, 200);
	m_victory_window.setSize(sf::Vector2f(ScreenWidth - 600, ScreenHeight - 400));
	texture = m_textureManager->LoadTexture("assets/sprites/items/particle_dead.png");
	m_next_wave_button = new GUI_Button(350, ScreenHeight-250, nullptr, texture, sf::IntRect(0,0,50,50));
	m_next_wave_button->Refresh();
	m_back_to_menu_button = new GUI_Button(1300, ScreenHeight - 250, nullptr, texture, sf::IntRect(50, 0, 50, 50));
	m_back_to_menu_button->Refresh();
	m_next_state = STATE_MENU;
	m_status = MODE_PLAYING;
}
GameState::~GameState()
{
	//Delete all bubbles
	for (int i = 0; i < m_bubbles.size(); i++)
	{
		delete m_bubbles[i];
		m_bubbles[i] = nullptr;
	}
	m_bubbles.clear();
	m_activeItems.clear();

	//Delete all monsters
	auto it = m_monsters.begin();
	while (it != m_monsters.end())
	{
		delete *it;
		++it;
	}
	m_monsters.clear();

	//Delete all waves
	auto itr = m_waves.begin();
	while (itr != m_waves.end())
	{
		delete *itr;
		++itr;
	}
	m_waves.clear();

	//Delete player
	if (m_player)
	{
		delete m_player;
		m_player = nullptr;
	}

	//Delete managers
	if (m_wordManager)
	{
		delete m_wordManager;
		m_wordManager = nullptr;
	}
		
	if (m_itemManager)
	{
		delete m_itemManager;
		m_itemManager = nullptr;
	}
		
	if (m_waveManager)
	{
		delete m_waveManager;
		m_waveManager = nullptr;
	}	

	if (m_powerUpManager)
	{
		delete m_powerUpManager;
		m_powerUpManager = nullptr;
	}
}

bool GameState::Update(float deltaTime)
{
	switch (m_status)
	{
		case MODE_PLAYING:
			return PlayMode(deltaTime);

		case MODE_VICTORY:
			return VictoryMode(deltaTime);

		case MODE_DEFEAT:
			return true;

		default:
			return true;
	}
}
void GameState::CheckCollision()
{
	if (m_status == MODE_PLAYING)
	{
		//Collision between items and monsters
		for (int i = 0; i < m_activeItems.size(); i++)
		{
			Item* item = m_activeItems[i];
			if (!item->IsActive())
				continue;

			for (int j = 0; j < m_monsters.size(); j++)
			{
				Monster* monster = m_monsters[j];
				if (!monster->IsActive())
					continue;
				if (CollisionManager::Check(monster->GetCollider(), item->GetCollider()))
				{
					if (m_powerUpManager->GetPierce())
					{
						if (m_powerUpManager->AddItemToPierceList(monster))
						{
							monster->Damage(item->GetProperty(), m_score);
						}
					}
					else if (m_powerUpManager->BounceItem() == item)
					{
						Monster* targetMonster = m_powerUpManager->NextBounceTarget();
						if (targetMonster == monster)
						{
							monster->Damage(item->GetProperty(), m_score);
							m_powerUpManager->NextBounce(monster);
							m_powerUpManager->AddLaneToBounceList(static_cast<int>(monster->GetX()));
						}
						else if (targetMonster == nullptr)
						{
							monster->Damage(item->GetProperty(), m_score);
							m_powerUpManager->NextBounce(monster);
							m_powerUpManager->AddLaneToBounceList(static_cast<int>(monster->GetX()));
						}

						//If sats som kollar ifall det är första gången bounceitem träffar monster, om true gör damage.
					}
					else
					{
						monster->Damage(item->GetProperty(), m_score);

						item->SetActive(false);
						item->SetInGame(false);
						item->SetState(ITEM_HIT);
					}

					if (monster->IsActive() == false)
					{
						//Increase score
						m_score += 100;
					}
				}
			}
		}

		//Collision between monsters and player
		for (int i = 0; i < m_monsters.size(); i++)
		{
			if (CollisionManager::Check(m_monsters[i]->GetCollider(), m_player->GetCollider()))
			{
				m_player->Knockdown();
			}
		}

		//Cleanup

		//Remove active items
		for (int i = 0; i < m_activeItems.size(); i++)
		{
			if (!m_activeItems[i]->IsActive())
			{
				m_activeItems.erase(m_activeItems.begin() + i);
			}
		}
	}
}

void GameState::Draw()
{
	//Draw background
	if (!m_powerUpManager->GetFrozen())
	{
		m_drawManager->Draw(m_backgroundSprite, sf::RenderStates::Default);
	}
	else
	{
		m_drawManager->Draw(m_ice_backgroundSprite, sf::RenderStates::Default);
	}

	//Draw Particles
	m_particleManager->Draw(m_drawManager);

	//Draw waves
	for (int i = 0; i < m_waves.size(); i++)
	{
		if (!m_waves[i]->IsActive())
			continue;
		m_waves[i]->Draw(m_drawManager);
	}

	//Draw monster
	for (int i = 0; i < m_monsters.size(); i++)
	{
		if (m_monsters[i]->IsActive())
		{
			m_monsters[i]->Draw(m_drawManager);
		}
	}

	//Draw player
	m_player->Draw(m_drawManager);

	//Draw bubbles
	for (int i = 0; i < m_bubbles.size(); i++)
	{
		m_bubbles[i]->Draw(m_drawManager);
	}

	//Draw typeable words
	m_wordManager->Draw(m_drawManager);

	//Draw active items
	for (int i = 0; i < m_activeItems.size(); i++)
	{
		if (m_activeItems[i]->IsActive())
		{
			m_activeItems[i]->Draw(m_drawManager);
		}
	}

	//Draw HUD
	for (int i = 0; i < m_life; i++)
	{
		m_life_sprite.setPosition(15.0f + 100.0f * i, 15);
		m_drawManager->Draw(m_life_sprite, sf::RenderStates::Default);
	}

	m_drawManager->Draw(m_score_sign_sprite, sf::RenderStates::Default);
	m_drawManager->Draw(m_scoreDisplay, sf::RenderStates::Default);

	//Draw Victory and Losing screen
	if (m_status == MODE_VICTORY)
	{
		m_drawManager->Draw(m_victory_window, sf::RenderStates::Default);
		m_next_wave_button->Draw(m_drawManager);
		m_back_to_menu_button->Draw(m_drawManager);
	}
	else if (m_status == MODE_DEFEAT)
	{
	}
}
void GameState::Enter()
{
	m_status = MODE_PLAYING;
}
void GameState::Exit()
{
	m_status = MODE_UNKNOWN;
}
ScreenState GameState::NextState()
{
	return m_next_state;
}

void GameState::InstantiateBubbles()
{
	//Instantiate thought bubbles
	sf::Texture* texture = m_textureManager->LoadTexture("assets/sprites/background/bubbles_spritesheet.png");
	for (int i = 0; i < 3; i++)
	{
		int yOffset = 0;
		if (i == 1)
			yOffset = 50;
		Bubble* bubble = new Bubble(725.0f + i * 200.0f, 910.0f + yOffset, texture, m_player);

		Item* item = m_itemManager->GetItem();

		bubble->SetItem(item);
		m_wordManager->SetNewWord(item->GetName());

		m_bubbles.push_back(bubble);
	}
}
void GameState::InstantiateMonsters()
{
	sf::Texture* undeadTexture = m_textureManager->LoadTexture("assets/sprites/monster/undead_monster_spritesheet.png");
	sf::Texture* lavaTexture = m_textureManager->LoadTexture("assets/sprites/monster/lava_monster_spritesheet.png");
	sf::Texture* aliveTexture = m_textureManager->LoadTexture("assets/sprites/monster/alive_monster_spritesheet.png");
	sf::Texture* iceTexture = m_textureManager->LoadTexture("assets/sprites/monster/ice_monster_spritesheet.png");

	sf::Texture* particleTexture = m_textureManager->LoadTexture("assets/sprites/particle.png");

	sf::SoundBuffer* monsterHitBuffer = m_audioManager->LoadSoundFromFile("assets/audio/complete/Monster_hurt01.wav");
	sf::SoundBuffer* monsterHitBufferTwo = m_audioManager->LoadSoundFromFile("assets/audio/complete/Monster_hurt02.wav");
	sf::SoundBuffer* monsterHitBufferThree = m_audioManager->LoadSoundFromFile("assets/audio/complete/Monster_hurt03.wav");
	//Monster pool
	for (int i = 0; i < 20; i++)
	{
		if (i < 5)
		{
			Monster* undeadMonster = new Monster(undeadTexture, "assets/sprites/monster/undead_monster_animation.txt", 225, 238, 45, ITEM_ALIVE, particleTexture);
			undeadMonster->SetSounds(monsterHitBuffer, monsterHitBufferTwo, monsterHitBufferThree);
			m_monsters.push_back(undeadMonster);
		}
		else if (i > 4 && i < 10)
		{
			Monster* lavaMonster = new Monster(lavaTexture, "assets/sprites/monster/lava_monster_animation.txt", 200, 227, 45, ITEM_COLD, particleTexture);
			lavaMonster->SetSounds(monsterHitBuffer, monsterHitBufferTwo, monsterHitBufferThree);
			m_monsters.push_back(lavaMonster);
		}
		else if (i > 9 && i < 15)
		{
			Monster* aliveMonster = new Monster(aliveTexture, "assets/sprites/monster/alive_monster_animation.txt", 207, 207, 45, ITEM_DEAD, particleTexture);
			aliveMonster->SetSounds(monsterHitBuffer, monsterHitBufferTwo, monsterHitBufferThree);
			m_monsters.push_back(aliveMonster);
		}
		else if (i > 14)
		{
			Monster* iceMonster = new Monster(iceTexture, "assets/sprites/monster/ice_monster_animation.txt", 226, 220, 45, ITEM_HOT, particleTexture);
			iceMonster->SetSounds(monsterHitBuffer, monsterHitBufferTwo, monsterHitBufferThree);
			m_monsters.push_back(iceMonster);
		}
	}
}
void GameState::SpawnMonster()
{
	while (true)
	{
		int randomMonster = rand() % 20;
		Monster* monster = m_monsters[randomMonster];
		if (!monster->IsActive())
		{
			monster->Activate();
			break;
		}
	}

	/*Monster* monster = new Monster(m_monsterTexture, 45, 3);
	m_monsters.push_back(monster);*/
}
void GameState::ConvertWordToItem()
{
	//Get spawned item from player
	Item* spawnedItem = m_player->GetItem();

	//If the item is not spawned, check if a word is complete and spawn it.
	if (!spawnedItem)
	{
		std::string finishedWord = m_wordManager->GetFinishedWord();
		if (finishedWord.size() == 0)
			return;

		for (int i = 0; i < m_bubbles.size(); i++)
		{
			Item* item = m_bubbles[i]->GetItem();

			if (item->GetName() != finishedWord)
				continue;

			Item* newItem = m_itemManager->GetItem();

			m_player->SetItem(item);
			m_wordManager->SetNewWord(newItem->GetName());
			m_bubbles[i]->SetItem(newItem);
			m_score += 200;

			m_conjureCompleteSound.play();
			break;
		}
	}
	else //If an item is spawned, activate it by pressing ENTER
	{
		if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Return))
		{
			//Activate item
			spawnedItem->Activate();
			m_activeItems.push_back(spawnedItem);
			m_player->SetItem(nullptr);
		}
	}
}

bool GameState::IsMonsters()
{
	//Check if there is any active monsters and return the result
	for (int i = 0; i < m_monsters.size(); i++)
	{
		if (m_monsters[i]->IsActive())
			return true;
	}

	return false;
}

bool GameState::PlayMode(float deltaTime)
{
	m_particleManager->Update(deltaTime);
	//Handle word input
	if (m_player->GetItem() == nullptr && !m_player->IsStunned())
	{
		m_wordManager->Update(deltaTime);
	}

	//Update wave manager
	m_waveManager->Update(deltaTime);
	if (m_waveManager->CanSpawnMonster())
	{
		SpawnMonster();
	}

	//Update active items
	for (int i = 0; i < m_activeItems.size(); i++)
	{
		if (!m_activeItems[i]->IsActive())
			continue;

		m_activeItems[i]->Update(deltaTime);
	}

	
	//Item movement
	for (int i = 0; i < m_activeItems.size(); i++)
	{
		if (!m_activeItems.at(i)->IsActive())
			continue;

			if (m_powerUpManager->BounceItem() == nullptr)
				m_activeItems.at(i)->Move(0, -m_speed * deltaTime);
			else
			{
				if (m_powerUpManager->NextBounceTarget() == nullptr)
					m_powerUpManager->BounceItem()->Move(0, -m_speed * deltaTime);
				else
				{
					sf::Vector2f itemDir = m_powerUpManager->ItemDirection();

					itemDir *= m_speed * deltaTime;
					m_powerUpManager->BounceItem()->Move(itemDir.x, itemDir.y);
				}
			
			}
	}


	//Update bubbles
	for (int i = 0; i < m_bubbles.size(); i++)
	{
		m_bubbles[i]->Update(deltaTime);
		m_wordManager->SetWordPosition(m_bubbles[i]->GetPosition(), i);

		bool active = m_wordManager->GetActiveBubbles().at(i);
		int alpha = (active) ? 255 : 80;
		m_bubbles[i]->SetAlpha(alpha);
	}

	//Update player
	m_player->Update(deltaTime);

	//Update monsters
	for (int i = 0; i < m_monsters.size(); i++)
	{
		if (!m_monsters[i]->IsActive())
			continue;

		m_monsters[i]->Update(deltaTime);

		//Activate burst
		if (m_monsters[i]->GetY() >= 775 && m_monsters[i]->Burst())
		{
			if (m_life > 0)
			{
				m_life -= 1;

				if (m_life == 0)
				{

				}
			}
		}
	}

	//Update waves
	m_waveTimer += deltaTime;
	for (int i = 0; i < m_waves.size(); i++)
	{
		if (!m_waves[i]->IsActive())
		{
			if (m_waveTimer >= 6)
			{
				m_waves[i]->Activate();
				m_waveTimer = 0;
			}
			else
			{
				continue;
			}
		}

		m_waves[i]->Update(deltaTime);
	}

	//Convert written words into item
	ConvertWordToItem();

	//Check collision
	CheckCollision();

	//Increase score if player enters correct key
	if (m_wordManager->GetCorrectKey())
	{
		m_score += 10;

		//Chanting animation
		m_player->ChantingAnimation();
	}

	if (m_score != m_lastScore)
		m_scoreDisplay.setString(std::to_string(m_score));

	m_lastScore = m_score;

	m_powerUpManager->Update(deltaTime);


	if (m_life <= 0)
	{

	}
	else if (!m_waveManager->IsActive() && !IsMonsters())
	{
		m_status = MODE_VICTORY;
	}

	return true;
}
bool GameState::VictoryMode(float deltaTime)
{
	m_next_wave_button->Update();
	m_back_to_menu_button->Update();

	if (m_next_wave_button->IsPressed())
	{
	}
	else if (m_back_to_menu_button->IsPressed())
	{
		return false;
	}
	return true;
}