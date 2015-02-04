#include "stdafx.h"
#include "GameState.h"

#include "ServiceLocator.h"
#include "DrawManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "CollisionManager.h"
#include "WordManager.h"
#include "ItemManager.h"

#include "Monster.h"
#include "Collider.h"
#include "Item.h"
#include "Player.h"

GameState::GameState()
{
	//Initiallize managers
	m_textureManager = ServiceLocator<TextureManager>::GetService();
	m_drawManager = ServiceLocator<DrawManager>::GetService();
	m_inputManager = ServiceLocator<InputManager >::GetService();
	m_wordManager = new WordManager();
	m_itemManager = new ItemManager();

	//Count the size of m_items
	m_itemCount = sizeof(m_items) / sizeof(m_items[0]);

	//Apply starting words
	for (int i = 0; i < m_itemCount; i++)
	{
		m_items[i] = m_itemManager->GetItem();
		m_wordManager->SetNewWord(m_items[i]->GetName());
	}

	//Load background sprite
	sf::Texture* texture = m_textureManager->LoadTexture("assets/sprites/background.png");
	m_backgroundSprite.setTexture(*texture);

	//Instantiate player
	texture = m_textureManager->LoadTexture("assets/sprites/wizard.png");
	m_player = new Player(texture);
}
GameState::~GameState()
{
	m_drawManager = nullptr;
	m_textureManager = nullptr;
	m_inputManager = nullptr;
	m_wordManager = nullptr;
	m_itemManager = nullptr;

	//Set all bubble items to nullptr
	for (int i = 0; i < m_itemCount; i++)
	{
		m_items[i] = nullptr;
	}

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

		//Remove items that gets above the screen
		if (m_activeItems[i]->GetY() <= -150)
		{
			m_activeItems[i]->SetActive(false);
			m_activeItems.erase(m_activeItems.begin() + i);
		}
	}

	//Update player
	m_player->Update(deltaTime);
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

		if (finishedWord.size() != 0)
		{
			for (int i = 0; i < m_itemCount; i++)
			{
				Item* item = m_items[i];

				if (item->GetName() == finishedWord)
				{
					//Spawn item
					m_player->SetItem(item);

					//Get new item and word
					while (true)
					{
						Item* newItem = m_itemManager->GetItem();
						
						//Check for duplicate words
						bool duplicateFound = false;
						for (int j = 0; j < m_itemCount; j++)
						{
							if (m_items[j]->GetName() == newItem->GetName())
							{
								duplicateFound = true;
							}
						}

						//If there was no duplicate, add the new word and item
						if (!duplicateFound)
						{
							m_items[i] = newItem;
							m_wordManager->SetNewWord(m_items[i]->GetName());
							break;
						}
					}
				}
			}
		}
	}
	else //If an item is spawned, activate it by pressing space
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

	//Draw typeable words
	m_wordManager->Draw(m_drawManager);


	//Draw active items
	for (int i = 0; i < m_activeItems.size(); i++)
	{
		if (m_activeItems[i]->IsActive())
			m_activeItems[i]->Draw(m_drawManager);
	}

	//Draw player
	m_player->Draw(m_drawManager);
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