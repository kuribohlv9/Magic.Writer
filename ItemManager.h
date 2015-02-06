#pragma once
#include "GameObject.h"

class TextureManager;
class Item;

class ItemManager
{
public:
	ItemManager();
	~ItemManager();

	Item* GetItem();

private:
	void AddItems(const std::string& spritesheetFilename, const std::string& textFilename);
	void GetProperties(int spriteSheetRow, ItemProperties& propertyOne, ItemProperties& propertyTwo);

private:
	std::vector<Item*> m_items;
	TextureManager* m_textureManager;
};

