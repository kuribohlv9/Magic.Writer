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

	//Set spawned item to nullptr initially
	m_spawnedItem = nullptr;
}
GameState::~GameState()
{
	m_drawManager = nullptr;
	m_textureManager = nullptr;
	m_inputManager = nullptr;
	m_wordManager = nullptr;
	m_itemManager = nullptr;
	m_spawnedItem = nullptr;

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
	if (m_spawnedItem == nullptr)
	{
		m_wordManager->Update(deltaTime);
	}

	//Convert written words into item
	ConvertWordToItem();
	

	//Remove items that gets above the screen
	for (int i = 0; i < m_activeItems.size(); i++)
	{
		m_activeItems[i]->Update(deltaTime);

		if (m_activeItems[i]->GetY() <= -150)
		{
			m_activeItems[i]->SetActive(false);
			m_activeItems.erase(m_activeItems.begin() + i);
		}
	}
	return true;
}
void GameState::ConvertWordToItem()
{
	//If an item is not spawned, check if a word is complete and spawn it.
	if (!m_spawnedItem)
	{
		std::string finishedWord = m_wordManager->GetFinishedWord();

		if (finishedWord.size() != 0)
		{
			for (int i = 0; i < m_itemCount; i++)
			{
				Item* item = m_items[i];

				if (item->GetName() == finishedWord)
				{
					m_spawnedItem = item;
					m_spawnedItem->SetPosition(350, 300);

					//Get new items
					m_items[i] = m_itemManager->GetItem();
					m_wordManager->SetNewWord(m_items[i]->GetName());
				}
			}
		}
	}
	else //If an item is spawned, activate it by pressing space
	{
		if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::Return))
		{
			//Activate item
			m_spawnedItem->Activate(350, 300);
			m_activeItems.push_back(m_spawnedItem);
			m_spawnedItem = nullptr;
		}
	}
}
void GameState::Draw()
{
	//Draw spawned item
	if (m_spawnedItem != nullptr)
	{
		m_spawnedItem->Draw(m_drawManager);
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