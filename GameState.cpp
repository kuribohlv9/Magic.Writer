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
#include "PowerManager.h"
#include "ParticleManager.h"
#include "HighscoreManager.h"
#include "BubbleManager.h"

//Classes
#include "Monster.h"
#include "Collider.h"
#include "Item.h"
#include "Player.h"
#include "Bubble.h"
#include "Utility.h"
#include "ParticleEmitter.h"
#include "GUI_Button.h"
#include "GUI_Label.h"


GameState::GameState()
{
	srand(time(NULL));

	//Initiallize managers
	m_textureManager = ServiceLocator<TextureManager>::GetService();
	m_drawManager = ServiceLocator<DrawManager>::GetService();
	m_inputManager = ServiceLocator<InputManager >::GetService();
	m_audioManager = ServiceLocator<AudioManager>::GetService();
	m_highscoreManager = ServiceLocator<HighscoreManager>::GetService();
	m_particleManager = ServiceLocator<ParticleManager>::GetService();

	//Load background texture
	sf::Texture* texture = m_textureManager->LoadTexture("assets/sprites/background.png");
	m_backgroundSprite.setTexture(*texture);
	texture = m_textureManager->LoadTexture("assets/sprites/ice_background.png");
	m_ice_backgroundSprite.setTexture(*texture);

	//Load HUD
	m_font = m_textureManager->LoadFont("assets/fonts/game.ttf");
	m_scoreDisplay.setFont(*m_font);
	m_scoreDisplay.setCharacterSize(40);
	m_scoreDisplay.setPosition(1645, 935);
	m_scoreDisplay.setColor(sf::Color(141, 119, 103, 255));
	m_userInfoText.setFont(*m_font);
	m_userInfoText.setPosition(450, ScreenHeight / 2 - 150);
	m_userInfoText.setColor(sf::Color(141, 119, 103, 255));
	m_userInfoText.setCharacterSize(25);

	//Load Music
	sf::Music* music = m_audioManager->LoadMusicFromFile("assets/Audio/Soundtracks - Theme & Bakground/Dubakupado.ogg");
	music->setVolume(20);
	m_game_themes.push_back(music);
	music = m_audioManager->LoadMusicFromFile("assets/Audio/Soundtracks - Theme & Bakground/AngloZulu.ogg");
	music->setVolume(20);
	m_game_themes.push_back(music);
	music = m_audioManager->LoadMusicFromFile("assets/Audio/Soundtracks - Theme & Bakground/Rite_of_Passage.ogg");
	music->setVolume(20);
	m_game_themes.push_back(music);
	music = m_audioManager->LoadMusicFromFile("assets/Audio/Soundtracks - Theme & Bakground/Whimsy_Groove.ogg");
	music->setVolume(20);
	m_game_themes.push_back(music);
	music = m_audioManager->LoadMusicFromFile("assets/Audio/Soundtracks - Theme & Bakground/Zanzibar.ogg");
	music->setVolume(20);
	m_game_themes.push_back(music);

	music = m_audioManager->LoadMusicFromFile("assets/Audio/Game_Over_-_Banjo-Kazooie.ogg");
	m_losing_theme = music;
}
GameState::~GameState()
{
	Exit();
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
			return DefeatMode(deltaTime);

		case MODE_READY:
			return ReadyMode(deltaTime);

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
					bool damageMonster = false;

					if (m_powerManager->GetPierceItem() == item) //Item pierce collision
					{
						if (m_powerManager->AddItemToPierceList(monster))
						{
							damageMonster = true;
						}
					}
					else if (m_powerManager->GetBounceItem() == item) //Item bounce collision
					{
						Monster* targetMonster = m_powerManager->GetBounceTarget();
						if (targetMonster == monster)
						{
							damageMonster = true;

							if (!m_powerManager->SetNewBounceTarget(monster))
							{
								item->SetActive(false);
								item->SetInGame(false);
								item->SetState(ITEM_HIT);
							}
						}
						else if (targetMonster == nullptr)
						{
							damageMonster = true;

							if (!m_powerManager->SetNewBounceTarget(monster))
							{
								item->SetActive(false);
								item->SetInGame(false);
								item->SetState(ITEM_HIT);
							}
						}
					}
					else //Item normal collision
					{
						//Damage and check for crit
						damageMonster = true;

						item->SetActive(false);
						item->SetInGame(false);
						item->SetState(ITEM_HIT);
					}

					if (damageMonster)
					{
						bool oneShot = false;
						//Damage monster
						monster->Damage(item->GetProperty(), oneShot);

						//Check for critical
						if (oneShot)
						{
							m_userInfo.currentScore += m_userInfo.criticalHitScore;
							m_userInfo.oneShots++;
						}
						else
						{
							m_userInfo.currentScore += m_userInfo.neutralHitScore;
						}

						//Monster is dead
						if (monster->IsDead())
						{
							//Increase score
							m_userInfo.currentScore += m_userInfo.monsterDefeatedScore;
							m_userInfo.monstersDefeated++;
						}
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
	if (!m_powerManager->IsFrozen())
	{
		m_drawManager->Draw(m_backgroundSprite, sf::RenderStates::Default);
	}
	else
	{
		m_drawManager->Draw(m_ice_backgroundSprite, sf::RenderStates::Default);
	}


	//Draw life
	if (m_life > 2)
	{
		m_drawManager->Draw(m_lifeSprite, sf::RenderStates::Default);
	}
	if (m_life > 1)
	{
		m_drawManager->Draw(m_lifeSprite2, sf::RenderStates::Default);
	}
	if (m_life > 0)
	{
		m_drawManager->Draw(m_lifeSprite3, sf::RenderStates::Default);
	}

	//Draw Particles
	m_particleManager->Draw(m_drawManager);

	//Draw HUD
	m_powerManager->Draw(m_drawManager);

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

	//BubbleManager
	m_bubbleManager->Draw(m_drawManager);

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

	m_drawManager->Draw(m_scoreDisplay, sf::RenderStates::Default);

	//Draw Victory and Losing screen
	if (m_status == MODE_VICTORY)
	{
		m_drawManager->Draw(m_victory_window, sf::RenderStates::Default);
		m_next_wave_button->Draw(m_drawManager);
		m_back_to_menu_button->Draw(m_drawManager);
		m_drawManager->Draw(m_userInfoText, sf::RenderStates::Default);
	}
	else if (m_status == MODE_DEFEAT)
	{
		m_drawManager->Draw(m_defeat_window, sf::RenderStates::Default);
		m_back_to_menu_button->Draw(m_drawManager);
		m_submit_button->Draw(m_drawManager);
		m_drawManager->Draw(m_userTextBox, sf::RenderStates::Default);
		m_drawManager->Draw(m_userInfoText, sf::RenderStates::Default);
	}
	if (m_status == MODE_READY)
	{
		//test
		m_drawManager->Draw(m_ready_text, sf::RenderStates::Default);
	}

}
void GameState::Enter()
{
	//Initialize managers
	m_wordManager = new WordManager();
	m_itemManager = new ItemManager();
	m_waveManager = new WaveManager();

	//Instantiate player
	sf::Texture* particleTexture = m_textureManager->LoadTexture("assets/sprites/wizard/particle.png");
	sf::Texture* sandParticle = m_textureManager->LoadTexture("assets/sprites/wizard/sand_particle.png");
	sf::Texture* texture = m_textureManager->LoadTexture("assets/sprites/wizard/wizard_spritesheet.png");
	sf::SoundBuffer* buffer = m_audioManager->LoadSoundFromFile("assets/audio/complete/Wizard_walk_sound.wav");
	m_player = new Player(texture, particleTexture, sandParticle, buffer);
	
	//Load sound
	buffer = m_audioManager->LoadSoundFromFile("assets/audio/complete/Wizard_spell_complete01.wav");
	m_conjureCompleteSound.setBuffer(*buffer);
	m_conjureCompleteSound.setVolume(7);


	m_powerManager = new PowerManager(&m_monsters, &m_activeItems, m_player);
	m_bubbleManager = new BubbleManager(m_player, m_wordManager);

	//Create bubbles and monster pool
	InstantiateMonsters();
	InstantiateBubbles();

	//Victory and Losing screen
	texture = m_textureManager->LoadTexture("assets/sprites/victory_screen.png");
	m_victory_window.setPosition(300, 200);
	m_victory_window.setTexture(*texture);
	m_victory_window.setTextureRect(sf::IntRect(0, 0, 1320, 680));

	m_next_wave_button = new GUI_Button(475, ScreenHeight - 275, nullptr, texture, sf::IntRect(0, 670, 261, 163));
	m_next_wave_button->Refresh();

	texture = m_textureManager->LoadTexture("assets/sprites/defeat_screen.png");
	m_defeat_window.setPosition(300, 200);
	m_defeat_window.setTexture(*texture);
	m_defeat_window.setTextureRect(sf::IntRect(0, 0, 1320, 680));
	
	m_back_to_menu_button = new GUI_Button(1445, ScreenHeight - 275, nullptr, texture, sf::IntRect(196, 683, 285, 163));
	m_back_to_menu_button->Refresh();
	m_submit_button = new GUI_Button(750, ScreenHeight - 450, nullptr, texture, sf::IntRect(1, 683, 183, 84));

	//Life sprite
	texture = m_textureManager->LoadTexture("assets/sprites/plupp_collection.png");
	m_lifeSprite.setTexture(*texture);
	m_lifeSprite.setOrigin(texture->getSize().x / 2, 0);
	m_lifeSprite.setPosition(ScreenWidth - 125, 0);

	m_lifeSprite2.setTexture(*texture);
	m_lifeSprite2.setOrigin(texture->getSize().x / 2, 0);
	m_lifeSprite2.setColor(sf::Color(241, 132, 132, 255));
	m_lifeSprite2.setPosition(ScreenWidth - 125, texture->getSize().y);

	m_lifeSprite3.setTexture(*texture);
	m_lifeSprite3.setOrigin(texture->getSize().x / 2, 0);
	m_lifeSprite3.setPosition(ScreenWidth - 125, texture->getSize().y * 2);

	//Highscore input
	m_userName = "";
	m_userTextBox.setFont(*m_font);
	m_userTextBox.setCharacterSize(40);
	m_userTextBox.setPosition(420, ScreenHeight - 465);
	m_userTextBox.setString(m_userName);
	m_userTextBox.setColor(sf::Color(110, 81, 35, 255));
	m_scoreDisplay.setString("0");

	//Instantsiate game variables
	m_userInfo.monstersDefeated = 0;
	m_userInfo.oneShots = 0;
	m_userInfo.currentScore = 0;
	m_userInfo.totalScore = 0;
	m_lastScore = 0;
	m_life = 3;
	m_speed = 800;
	m_wave_level = 1;
	m_status = MODE_PLAYING;
	m_next_state = STATE_MENU;
	m_waveManager->SetActiveWave(0);

	//Readymode
	GotoReady();
}
void GameState::Exit()
{
	//Delete particles
	m_particleManager->Reset();

	//Delete Managers
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
	if (m_powerManager)
	{
		delete m_powerManager;
		m_powerManager = nullptr;
	}

	//Delete player
	if (m_player)
	{
		delete m_player;
		m_player = nullptr;
	}

	m_activeItems.clear();

	//Delete monsters
	auto itr = m_monsters.begin();
	while (itr != m_monsters.end())
	{
		if (*itr)
		{
			delete *itr;
			*itr = nullptr;
		}
		itr++;
	}
	m_monsters.clear();

	if (m_back_to_menu_button)
	{
		delete m_back_to_menu_button;
		m_back_to_menu_button = nullptr;
	}
	if (m_next_wave_button)
	{
		delete m_next_wave_button;
		m_next_wave_button = nullptr;
	}
	if (m_submit_button)
	{
		delete m_submit_button;
		m_submit_button = nullptr;
	}

	if (m_active_theme != nullptr)
		m_active_theme->stop();
}
ScreenState GameState::NextState()
{
	return m_next_state;
}

void GameState::InstantiateBubbles()
{
	//Instantiate thought bubble items
	for (int i = 0; i < 3; i++)
	{
		Item* item = m_itemManager->GetItem(true, true);

		m_bubbleManager->GetBubble(i)->SetItem(item);
		m_wordManager->SetNewWord(item->GetName());
	}
}
void GameState::InstantiateMonsters()
{
	sf::Texture* lavaTexture = m_textureManager->LoadTexture("assets/sprites/monster/lava_monster_spritesheet.png");
	sf::Texture* iceTexture = m_textureManager->LoadTexture("assets/sprites/monster/ice_monster_spritesheet.png");

	sf::Texture* particle = m_textureManager->LoadTexture("assets/sprites/monster/particle.png");

	sf::SoundBuffer* monsterHitBuffer = m_audioManager->LoadSoundFromFile("assets/audio/Misc/BAM.wav");
	sf::SoundBuffer* monsterHitBufferTwo = m_audioManager->LoadSoundFromFile("assets/audio/Misc/BAM.wav");
	sf::SoundBuffer* monsterHitBufferThree = m_audioManager->LoadSoundFromFile("assets/audio/Misc/BAM.wav");
	//Monster pool
	for (int i = 0; i < 10; i++)
	{
		if (i < 5)
		{
			Monster* lavaMonster = new Monster(lavaTexture, "assets/sprites/monster/lava_monster_animation.txt", 153, 173, 45, ITEM_COLD, particle);
			lavaMonster->SetSounds(monsterHitBuffer, monsterHitBufferTwo, monsterHitBufferThree);
			m_monsters.push_back(lavaMonster);
		}
		else if (i >=5 )
		{
			Monster* iceMonster = new Monster(iceTexture, "assets/sprites/monster/ice_monster_animation.txt", 182, 178, 45, ITEM_HOT, particle);
			iceMonster->SetSounds(monsterHitBuffer, monsterHitBufferTwo, monsterHitBufferThree);
			m_monsters.push_back(iceMonster);
		}
	}
}
void GameState::SpawnMonster()
{
	while (true)
	{
		int randomMonster = rand() % m_monsters.size();
		Monster* monster = m_monsters[randomMonster];
		if (!monster->IsActive())
		{
			monster->Activate(25 + 7*m_wave_level);
			break;
		}
	}
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

		for (unsigned int i = 0; i < 3; i++)
		{
			Bubble* bubble = m_bubbleManager->GetBubble(i);
			Item* item = bubble->GetItem();

			if (item->GetName() != finishedWord)
				continue;

			Item* newItem = m_itemManager->GetItem(AllowProperty(ITEM_HOT), AllowProperty(ITEM_COLD));

			m_player->SetItem(item);
			m_wordManager->SetNewWord(newItem->GetName());
			bubble->SetItem(newItem);

			m_conjureCompleteSound.play();

			//Increase power plupps
			m_powerManager->AddPowerupPlupps(2);
			break;
		}
	}
	else //If an item is spawned, activate it by pressing ENTER
	{
		if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Return) || m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Space))
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
void GameState::SetUserInfoVictory()
{
	std::string i = "";
	m_userInfoText.setCharacterSize(35);
	m_userInfoText.setPosition(430, ScreenHeight / 2 - 150);
	i += "Monsters defeated: " + std::to_string(m_userInfo.monstersDefeated);
	i += "\nOne hit kills: " + std::to_string(m_userInfo.oneShots);
	i += "\nLives left: " + std::to_string(m_life);
	i += "\nScore this wave: " + std::to_string(m_userInfo.currentScore);

	UpdateScore();
	i += "\nTotal score: " + std::to_string(m_userInfo.totalScore);
	m_userInfoText.setString(i);

	//Reset stuff
	m_userInfo.monstersDefeated = 0;
	m_userInfo.oneShots = 0;
}
void GameState::SetUserInfoDefeat()
{
	UpdateScore();

	m_userInfoText.setCharacterSize(40);
	m_userInfoText.setPosition(570, ScreenHeight / 2 - 50);
	m_userInfoText.setString(std::to_string(m_userInfo.totalScore));
}
void GameState::UpdateScore()
{
	m_userInfo.totalScore += m_userInfo.currentScore;
	m_userInfo.currentScore = 0;
}
bool GameState::AllowProperty(ItemProperty prop)
{
	for (unsigned int i = 0; i < 3; i++)
	{
		ItemProperty p = m_bubbleManager->GetBubble(i)->GetItem()->GetProperty();

		if (p == prop)
		{
			return false;
		}
	}

	return true;
}

bool GameState::PlayMode(float deltaTime)
{
	m_particleManager->Update(deltaTime);
	m_bubbleManager->Update(deltaTime);

	//Handle word input
	if (m_player->GetItem() == nullptr && !m_player->IsStunned())
	{
		m_wordManager->Update(deltaTime);
	}

	//Update wave manager
	if (!m_powerManager->IsFrozen())
	{
		m_waveManager->Update(deltaTime);
		if (m_waveManager->CanSpawnMonster())
		{
			SpawnMonster();
		}
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

		//Not a bounce item
		if (m_powerManager->GetBounceItem() != m_activeItems[i])
		{
			m_activeItems[i]->Move(0, -m_speed * deltaTime);
		}
		else//Item is bouncy
		{
			//No Target
			if (m_powerManager->GetBounceTarget() == nullptr)
			{
				m_activeItems[i]->Move(0, -m_speed * deltaTime);
			}
			else
			{
				sf::Vector2f itemDir = m_powerManager->GetItemDirection();

				itemDir *= m_speed * deltaTime;
				m_powerManager->GetBounceItem()->Move(itemDir.x, itemDir.y);
			}
		}
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
		if (m_monsters[i]->GetY() >= 775)
			m_monsters[i]->Burst();
			
		if (m_life > 0 && m_monsters[i]->GetY() >= ScreenHeight)
		{
			m_life -= 1;
		}
	}

	//Convert written words into item
	ConvertWordToItem();

	//Check collision
	CheckCollision();

	//Increase score if player enters correct key
	if (m_wordManager->GetCorrectKey())
	{
		//Chanting animation
		m_player->PlayChantingAnimation();
	}

	//Update score display
	if (m_userInfo.currentScore != m_lastScore)
	{
		m_scoreDisplay.setString(std::to_string(m_userInfo.currentScore));
		float width = m_scoreDisplay.getGlobalBounds().width;
		m_scoreDisplay.setPosition(1645 - width / 2, 935);
	}
	m_lastScore = m_userInfo.currentScore;

	m_powerManager->Update(deltaTime);

	if (!m_active_theme || m_active_theme->getStatus() == sf::Music::Status::Stopped)
	{
		m_active_theme = m_game_themes[rand() % 5];
		m_active_theme->play();
	}

	//Check win and lose condition
	if (m_life <= 0)
	{
		m_status = MODE_DEFEAT;
		SetUserInfoDefeat();
	}
	else if (!m_waveManager->IsActive() && !IsMonsters())
	{
		m_status = MODE_VICTORY;
		m_active_theme->stop();
		SetUserInfoVictory();
	}

	return true;
}
bool GameState::VictoryMode(float deltaTime)
{
	m_next_wave_button->Update();
	m_back_to_menu_button->Update();

	if (m_next_wave_button->IsPressed())
	{
		//increase wave level and start the next wave
		m_wave_level++;
		m_waveManager->SetActiveWave(m_wave_level);
		m_status = MODE_PLAYING;
		m_life = 3;
		GotoReady();
	}
	else if (m_back_to_menu_button->IsPressed())
	{
		return false;
	}
	return true;
}
bool GameState::DefeatMode(float deltaTime)
{
	if (m_losing_theme->getStatus() == sf::Music::Status::Stopped)
	{
		m_losing_theme->play();
	}

	m_back_to_menu_button->Update();
	m_submit_button->Update();

	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::BackSpace))
	{
		int nameSize = m_userName.size();
		if (nameSize > 0)
		{
			m_userName.erase(m_userName.begin() + nameSize - 1);
			m_userTextBox.setString(m_userName);
		}
	}
	else if (m_submit_button->IsPressed())
	{
		if (m_userName.size() > 0)
		{
			ScoreEntry entry;
			entry.name = m_userName;
			entry.score = m_userInfo.totalScore;
			m_highscoreManager->WriteHighscore(entry);
			m_losing_theme->stop();
			return false;
		}
	}
	else
	{
		char c = m_inputManager->GetInputChar();
		if (c != ' ' && m_userName.size() < 8)
		{
			m_userName += c;
			m_userTextBox.setString(m_userName);
		}
	}

	if (m_back_to_menu_button->IsPressed())
	{
		m_losing_theme->stop();
		return false;
	}
	return true;
}

bool GameState::ReadyMode(float deltaTime)
{
	m_ready_timer++;
	if (m_ready_timer > 180)
	{
		m_status = MODE_PLAYING;
	}
	return true;
}

void GameState::GotoReady()
{
	m_ready_string = "Wave " + std::to_string(m_wave_level) + " Start!";
	m_ready_text.setFont(*m_font);
	m_ready_text.setColor(sf::Color::Black);
	m_ready_text.setStyle(sf::Text::Bold);
	m_ready_text.setCharacterSize(40);
	m_ready_text.setString(m_ready_string);
	m_ready_text.setPosition(ScreenWidth/2 - m_ready_text.getLocalBounds().width/2, 100);

	m_ready_timer = 0;
	m_status = MODE_READY;
}