#include "stdafx.h"
#include "GameState.h"
#include <time.h>

//Manager
#include "ServiceLocator.h"
#include "DrawManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "CollisionManager.h"
#include "WordManager.h"
#include "ItemManager.h"
#include "WaveManager.h"

//Classes
#include "Monster.h"
#include "Collider.h"
#include "Item.h"
#include "Player.h"
#include "Bubble.h"
#include "Utility.h"

#include <sstream>

GameState::GameState()
{
	srand(time(NULL));

	//Initiallize managers
	m_textureManager = ServiceLocator<TextureManager>::GetService();
	m_drawManager = ServiceLocator<DrawManager>::GetService();
	m_inputManager = ServiceLocator<InputManager >::GetService();
	m_wordManager = new WordManager();
	m_itemManager = new ItemManager();
	m_waveManager = new WaveManager();

	//Load background texture
	sf::Texture* texture = m_textureManager->LoadTexture("assets/sprites/background.png");
	m_backgroundSprite.setTexture(*texture);
	m_backgroundSprite.setPosition(0, 0);

	//Load monster texture
	m_monsterTexture = m_textureManager->LoadTexture("assets/sprites/spritesheet_monster.png");

	//Instantiate player
	texture = m_textureManager->LoadTexture("assets/sprites/wizard/spritesheet_wizard.png");
	m_player = new Player(texture);

	//Instantiate thought bubbles
	texture = m_textureManager->LoadTexture("assets/sprites/bubbles_spritesheet.png");
	for (int i = 0; i < 3; i++)
	{
		Bubble* bubble = new Bubble(550 + i * 400 + i * 30, 960, texture);

		Item* item = m_itemManager->GetItem();

		bubble->SetItem(item);
		m_wordManager->SetNewWord(item->GetName());

		m_bubbles.push_back(bubble);

		//Load test font
		m_font = m_textureManager->LoadFont("assets/fonts/font.ttf");

		//Testing for HUD
		m_score = 0;
		m_scoreDisplay.setString("0");
		m_circle.setFillColor(sf::Color::Red);
		m_circle.setRadius(50);
		m_scoreDisplay.setFont(*m_font);
		m_scoreDisplay.setScale(3, 3);
		m_scoreDisplay.setPosition(1700, 900);
		m_life = 3;
	}
}
GameState::~GameState()
{
	m_drawManager = nullptr;
	m_textureManager = nullptr;
	m_inputManager = nullptr;
	m_wordManager = nullptr;
	m_itemManager = nullptr;

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


	//Delete player
	if (m_player)
	{
		delete m_player;
	}

	//Delete managers
	if (m_wordManager)
		delete m_wordManager;
	if (m_itemManager)
		delete m_itemManager;
	if (m_waveManager)
		delete m_waveManager;
}

bool GameState::Update(float deltaTime)
{
	//Handle word input if an item is not spawned
	if (m_player->GetItem() == nullptr)
	{
		m_wordManager->Update(deltaTime);
	}

	//Update waves
	m_waveManager->Update(deltaTime);
	if (m_waveManager->CanSpawnMonster())
	{
		SpawnMonster();
	}

	//Convert written words into item
	ConvertWordToItem();
	

	//Update active items
	for (int i = 0; i < m_activeItems.size(); i++)
	{
		if (!m_activeItems[i]->IsActive())
			break;

		m_activeItems[i]->Update(deltaTime);
	}

	//Update player
	m_player->Update(deltaTime);

	//Update monsters
	for (int i = 0; i < m_monsters.size(); i++)
	{
		m_monsters[i]->Update(deltaTime);
	}



	//CHECK COLLISION
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

			if (CollisionManager::Check(item->GetCollider(), monster->GetCollider()))
			{
				monster->Damage(item->GetProperty(), m_score);
				item->SetActive(false);
				item->SetInGame(false);

				if (monster->IsActive() == false)
				{
					//Update score
					m_score += 100;
				}
			}
		}
	}

	for (int i = 0; i < m_activeItems.size(); i++)
	{
		if (!m_activeItems[i]->IsActive())
		{
			m_activeItems.erase(m_activeItems.begin() + i);
		}
	}
	for (int i = 0; i < m_monsters.size(); i++)
	{
		if (!m_monsters[i]->IsActive())
		{
			delete m_monsters[i];
			m_monsters.erase(m_monsters.begin() + i);
		}
	}


	//Increase score if player enters correct key
	if (m_wordManager->GetCorrectKey())
	{
		m_score += 10;
	}


	if (m_score != m_lastScore)
		m_scoreDisplay.setString(std::to_string(m_score));

	m_lastScore = m_score;

	return true;
}
void GameState::Draw()
{
	//Draw background
	m_drawManager->Draw(m_backgroundSprite, sf::RenderStates::Default);

	//Draw monster
	for (int i = 0; i < m_monsters.size(); i++)
	{
		if (m_monsters[i]->IsActive())
			m_monsters[i]->Draw(m_drawManager);
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
			m_activeItems[i]->Draw(m_drawManager);
	}

	//Draw HUD
	for (int i = 0; i < m_life; i++)
	{
		m_circle.setPosition(60.0f + 100.0f * i, 900);
		m_drawManager->Draw(m_circle, sf::RenderStates::Default);
	}

	m_drawManager->Draw(m_scoreDisplay, sf::RenderStates::Default);
}

void GameState::Enter()
{

}
void GameState::Exit()
{

}
ScreenState GameState::NextState()
{
	return STATE_MENU;
}

void GameState::SpawnMonster()
{
	Monster* monster = new Monster(m_monsterTexture, 25.0f, 3);
	m_monsters.push_back(monster);
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
			m_score += 500;
			break;
		}
	}
	else //If an item is spawned, activate it by pressing ENTER
	{
		if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Return))
		{
			//Activate item
			spawnedItem->ActivateItem();
			m_activeItems.push_back(spawnedItem);
			m_player->SetItem(nullptr);
		}
	}
}