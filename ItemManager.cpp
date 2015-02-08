#include "stdafx.h"
#include "ItemManager.h"
#include "TextureManager.h"
#include "ServiceLocator.h"
#include "Item.h"

#include <fstream>

ItemManager::ItemManager()
{
	m_textureManager = ServiceLocator<TextureManager>::GetService();
	AddItems("assets/items/item_alive.png", "assets/items/item_alive.txt", ITEM_ALIVE);
	AddItems("assets/items/item_dead.png", "assets/items/item_dead.txt", ITEM_DEAD);
	AddItems("assets/items/item_hot.png", "assets/items/item_hot.txt", ITEM_HOT);
	AddItems("assets/items/item_cold.png", "assets/items/item_cold.txt", ITEM_COLD);
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

void ItemManager::AddItems(const std::string& spritesheetFilename, const std::string& textFilename, ItemProperty property)
{
	//Load spritesheet for items
	sf::Texture* itemSpriteSheet = m_textureManager->LoadTexture(spritesheetFilename);

	//Load item names
	std::ifstream stream;
	stream.open(textFilename);

	//Create a sourcerectangle (part of a texture)
	int itemWidthHeight = 128;
	sf::IntRect sourceRectangle = sf::IntRect(0, 0, itemWidthHeight, itemWidthHeight);

	//What row in the spritesheet we are on
	int row = 0;

	while (!stream.eof())
	{
		//Read item name from textfile
		std::string itemName;
		std::getline(stream, itemName);

		if (itemName == "-")
		{
			//Change spritesheet row
			sourceRectangle.left = 0;
			sourceRectangle.top += itemWidthHeight;
			row++;
		}
		else
		{
			Item* item = new Item(itemSpriteSheet, sourceRectangle, property, itemName);
			m_items.push_back(item);

			sourceRectangle.left += itemWidthHeight;
		}
	}
	stream.close();
}

Item* ItemManager::GetItem()
{
	while (true)
	{
		int randomIndex = rand() % m_items.size();

		Item* item = m_items[randomIndex];

		if (!item->IsActive() && !item->IsInGame())
		{
			item->SetInGame(true);
			return item;
		}
	}
}
