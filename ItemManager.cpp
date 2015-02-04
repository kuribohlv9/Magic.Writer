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
	AddItems();
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

void ItemManager::AddItems()
{
	//Load spritesheet for items
	sf::Texture* itemSpriteSheet = m_textureManager->LoadTexture("assets/sprites/item_spritesheet.png");

	//Load item names
	std::ifstream stream;
	stream.open("assets/text_files/item_names.txt");

	//Create a sourcerectangle (part of a texture)
	sf::IntRect sourceRectangle = sf::IntRect(0, 0, 150, 150);

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
			sourceRectangle.top += 150;
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

			sourceRectangle.left += 150;
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

	return m_items[randomIndex];

	//May erase returned item in future, and insert it back later
}
