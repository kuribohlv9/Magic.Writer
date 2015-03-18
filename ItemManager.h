#pragma once
#include "GameObject.h"

class TextureManager;
class Item;

class ItemManager
{
public:
	ItemManager();
	~ItemManager();

	Item* GetItem(bool allowHot, bool allowCold);

private:
	void AddItems(const std::string& spritesheetFilename, const std::string& textFilename, ItemProperty property, sf::Texture* particle);

private:
	std::vector<Item*> m_items;
	TextureManager* m_textureManager;
};

