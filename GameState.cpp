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
	m_monsterTexture = m_textureManager->LoadTexture("assets/sprites/monster/spritesheet_monster.png");

	//Instantiate player
	texture = m_textureManager->LoadTexture("assets/sprites/wizard/spritesheet_wizard.png");
	m_player = new Player(texture);

	//Instantiate thought bubbles
	texture = m_textureManager->LoadTexture("assets/sprites/bubbles_spritesheet.png");
	for (int i = 0; i < 3; i++)
	{
		int yOffset = 0;
		if (i == 1)
			yOffset = 50;
		Bubble* bubble = new Bubble(725 + i * 200, 910 + yOffset, texture, m_player);

		Item* item = m_itemManager->GetItem();

		bubble->SetItem(item);
		m_wordManager->SetNewWord(item->GetName());

		m_bubbles.push_back(bubble);
	}

	//HUD
	m_font = m_textureManager->LoadFont("assets/fonts/font.ttf");
	m_life_sprite.setTexture(*m_textureManager->LoadTexture("assets/sprites/HUD/life.png"));
	
	m_scoreDisplay.setFont(*m_font);
	m_scoreDisplay.setScale(1.5f, 1.5f);
	m_scoreDisplay.setPosition(1750, 950);
	m_scoreDisplay.setString("0");
	m_scoreDisplay.setColor(sf::Color(30, 58, 64, 255));

	m_score = 0;
	m_lastScore = 0;
	m_life = 3;
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

	for (int i = 0; i < m_bubbles.size(); i++)
	{
		m_bubbles[i]->Update(deltaTime);
		m_wordManager->SetWordPosition(m_bubbles[i]->GetPosition(), i);
	}

	//Update player
	m_player->Update(deltaTime);

	//Update monsters
	for (int i = 0; i < m_monsters.size(); i++)
	{
		m_monsters[i]->Update(deltaTime);
	}

	//Increase score if player enters correct key
	if (m_wordManager->GetCorrectKey())
	{
		m_score += 10;
	}

	//Convert written words into item
	ConvertWordToItem();

	//Check collision
	CheckCollision();


	if (m_score != m_lastScore)
		m_scoreDisplay.setString(std::to_string(m_score));

	m_lastScore = m_score;

	return true;
}
void GameState::CheckCollision()
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

			//Collision check
			if (CollisionManager::Check(item->GetCollider(), monster->GetCollider()))
			{
				monster->Damage(item->GetProperty(), m_score);
				item->SetActive(false);
				item->SetInGame(false);

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
	//Remove inactive monsters
	for (int i = 0; i < m_monsters.size(); i++)
	{
		if (!m_monsters[i]->IsActive())
		{
			delete m_monsters[i];
			m_monsters.erase(m_monsters.begin() + i);
		}
	}
}

void GameState::Draw()
{
	//Draw background
	m_drawManager->Draw(m_backgroundSprite, sf::RenderStates::Default);

	//Draw monster
	for (int i = 0; i < m_monsters.size(); i++)
	{
		if (m_monsters[i]->IsActive())
		{
			m_monsters[i]->Draw(m_drawManager);
			sf::RectangleShape shape;
			shape.setPosition(m_monsters[i]->GetCollider()->GetArea().left, m_monsters[i]->GetCollider()->GetArea().top);
			shape.setSize(sf::Vector2f(m_monsters[i]->GetCollider()->GetArea().width, m_monsters[i]->GetCollider()->GetArea().height));
			shape.setFillColor(sf::Color(0, 255, 0, 125));
			m_drawManager->Draw(shape, sf::RenderStates::Default);
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
		m_life_sprite.setPosition(60.0f + 100.0f * i, 900);
		m_drawManager->Draw(m_life_sprite, sf::RenderStates::Default);
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
	Monster* monster = new Monster(m_monsterTexture, 45, 3);
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
			m_score += 200;
			break;
		}
	}
	else //If an item is spawned, activate it by pressing ENTER
	{
		if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Return))
		{
			//Activate item
			spawnedItem->SetActive(true);
			m_activeItems.push_back(spawnedItem);
			m_player->SetItem(nullptr);
		}
	}
}