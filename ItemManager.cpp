#include "stdafx.h"
#include "ItemManager.h"
#include "TextureManager.h"
#include "ServiceLocator.h"
#include "Item.h"

#include <fstream>

ItemManager::ItemManager()
{
	m_textureManager = ServiceLocator<TextureManager>::GetService();

	sf::Texture* particle = m_textureManager->LoadTexture("assets/sprites/items/particle_alive.png");
	AddItems("assets/sprites/items/item_alive.png", "assets/sprites/items/item_alive.txt", ITEM_ALIVE, particle);

	particle = m_textureManager->LoadTexture("assets/sprites/items/particle_dead.png");
	AddItems("assets/sprites/items/item_dead.png", "assets/sprites/items/item_dead.txt", ITEM_DEAD, particle);

	particle = m_textureManager->LoadTexture("assets/sprites/items/particle_hot.png");
	AddItems("assets/sprites/items/item_hot.png", "assets/sprites/items/item_hot.txt", ITEM_HOT, particle);

	particle = m_textureManager->LoadTexture("assets/sprites/items/particle_cold2.png");
	AddItems("assets/sprites/items/item_cold.png", "assets/sprites/items/item_cold.txt", ITEM_COLD, particle);
}

ItemManager::~ItemManager()
{
	//Delete all items
	auto itr = m_items.begin();
	while (itr != m_items.end())
	{
		delete (*itr);
		++itr;
	}

	m_items.clear();
}

void ItemManager::AddItems(const std::string& spritesheetFilename, const std::string& textFilename, ItemProperty property, sf::Texture* particle)
{
	//Load spritesheet for items
	sf::Texture* itemSpriteSheet = m_textureManager->LoadTexture(spritesheetFilename);

	//Load item names
	std::ifstream stream;
	stream.open(textFilename);

	

	//Create a sourcerectangle (part of a texture)
	Item* item = new Item(itemSpriteSheet, particle, property, "");

	int frameCount = -1;
	while (!stream.eof())
	{
		if (frameCount < 0)
		{
			//Read item name from textfile
			std::string itemName;
			stream >> itemName;//std::getline(stream, itemName);
			item->SetName(itemName);
		}

		sf::IntRect sourceRectangle;
		stream >> sourceRectangle.left;
		stream >> sourceRectangle.top;
		stream >> sourceRectangle.width;
		stream >> sourceRectangle.height;
		frameCount++;

		item->SetSourceRectangle(frameCount, sourceRectangle);

		if (frameCount >= 2)
		{
			frameCount = -1;
			m_items.push_back(item);
			item = new Item(itemSpriteSheet, particle, property, "");
		}
	}

	delete item;
	 
	stream.close();
}

Item* ItemManager::GetItem()
{
	//Start an infinite loop.
	while (true)
	{
		//Randomize a index between 0 and the total item count.
		int randomIndex = rand() % m_items.size();

		//Access the item at randomized index.
		Item* item = m_items[randomIndex];

		//Check if the item is not active and is not inside the game.
		if (!item->IsActive() && !item->IsInGame())
		{
			//Return the item and set in game to true.
			item->SetInGame(true);
			return item;
		}
	}
}
