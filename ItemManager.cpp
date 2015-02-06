#include "stdafx.h"
#include "ItemManager.h"
#include "TextureManager.h"
#include "ServiceLocator.h"
#include "Item.h"

#include <fstream>
#include <time.h>


ItemManager::ItemManager()
{
	srand(time(NULL));

	m_textureManager = ServiceLocator<TextureManager>::GetService();
	AddItems("assets/items/item_spritesheet_0.png", "assets/items/item_names_0.txt");
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

void ItemManager::AddItems(const std::string& spritesheetFilename, const std::string& textFilename)
{
	//Load spritesheet for items
	sf::Texture* itemSpriteSheet = m_textureManager->LoadTexture(spritesheetFilename);

	//Load item names
	std::ifstream stream;
	stream.open(textFilename);

	//Create a sourcerectangle (part of a texture)
	int itemWidthHeight = 256;
	sf::IntRect sourceRectangle = sf::IntRect(0, 0, itemWidthHeight, itemWidthHeight);

	//What row in the spritesheet we are on
	int row = 0;

	while (!stream.eof())
	{
		//Read item name from textfile
		std::string itemName;
		stream >> itemName;

		if (itemName == "-")
		{
			//Change spritesheet row
			sourceRectangle.left = 0;
			sourceRectangle.top += itemWidthHeight;
			row++;
		}
		else
		{
			//Add an item
			ItemProperties propertyOne;
			ItemProperties propertyTwo;
			GetProperties(row, propertyOne, propertyTwo);

			Item* item = new Item(itemSpriteSheet, sourceRectangle, propertyOne, propertyTwo, itemName);
			m_items.push_back(item);

			sourceRectangle.left += itemWidthHeight;
		}
	}
	stream.close();
}
void ItemManager::GetProperties(int spriteSheetRow, ItemProperties& propertyOne, ItemProperties& propertyTwo)
{
	//Return properties depending on which spritesheetrow we are on
	switch (spriteSheetRow)
	{
	case 0:
		propertyOne = SOFT;
		propertyTwo = ALIVE;
		break;
	case 1:
		propertyOne = SOFT;
		propertyTwo = DEAD;
		break;
	case 2:
		propertyOne = HARD;
		propertyTwo = DEAD;
		break;
	case 3:
		propertyOne = HARD;
		propertyTwo = ALIVE;
		break;
	}
}

Item* ItemManager::GetItem()
{
	int randomIndex = rand() % m_items.size();

	Item* item = m_items[randomIndex];
	//m_items.erase(m_items.begin() + randomIndex);

	return item;
	//May erase returned item in future, and insert it back later
}
