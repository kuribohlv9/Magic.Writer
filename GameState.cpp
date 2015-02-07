#include "stdafx.h"
#include "GameState.h"

//Manager
#include "ServiceLocator.h"
#include "DrawManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "CollisionManager.h"
#include "WordManager.h"
#include "ItemManager.h"

//Classes
#include "Monster.h"
#include "Collider.h"
#include "Item.h"
#include "Player.h"
#include "Bubble.h"

GameState::GameState()
{
	//Initiallize managers
	m_textureManager = ServiceLocator<TextureManager>::GetService();
	m_drawManager = ServiceLocator<DrawManager>::GetService();
	m_inputManager = ServiceLocator<InputManager >::GetService();
	m_wordManager = new WordManager();
	m_itemManager = new ItemManager();

	//Load background sprite
	sf::Texture* texture = m_textureManager->LoadTexture("assets/sprites/background.png");
	m_backgroundSprite.setTexture(*texture);

	//Instantiate player
	texture = m_textureManager->LoadTexture("assets/sprites/wizard.png");
	m_player = new Player(texture);

	//Instantiate thought bubbles
	texture = m_textureManager->LoadTexture("assets/sprites/bubble_spritesheet.png");
	for (int i = 0; i < 3; i++)
	{
		Bubble* bubble = new Bubble(550 + i * 400 + i * 30, 960, texture);

		Item* item = m_itemManager->GetItem();

		bubble->SetItem(item);
		m_wordManager->SetNewWord(item->GetName());

		m_bubbles.push_back(bubble);
	}


	//TMP MONSTER SPAWNING
	texture = m_textureManager->LoadTexture("assets/sprites/spritesheet_monster.png");
	for (int i = 0; i < 5; i++)
	{
		int randomX = rand() % 5;
		sf::Vector2f position = sf::Vector2f(192 + 384 * randomX, -i * 300);

		Monster* monster = new Monster(texture, position.x, position.y, 25.0f, 5, ITEM_ALIVE);

		m_monsters.push_back(monster);
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

	//Set all active items to nullptr
	for (int i = 0; i < m_activeItems.size(); i++)
	{
		m_activeItems[i] = nullptr;
	}
	m_activeItems.clear();
}

bool GameState::Update(float deltaTime)
{
	//Handle word input if an item is not spawned
	if (m_player->GetItem() == nullptr)
	{
		m_wordManager->Update(deltaTime);
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
				monster->Damage(item->GetProperty());
				item->SetActive(false);
				item->SetInGame(false);
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
	return true;
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