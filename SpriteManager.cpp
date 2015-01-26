#include "stdafx.h"
#include "SpriteManager.h"

SpriteManager::SpriteManager()
{

}

SpriteManager::~SpriteManager()
{
	//Loop through all sprites and delete them
	auto itSprite = m_sprites.begin();
	while (itSprite != m_sprites.end())
	{
		delete (*itSprite);
		++itSprite;
	}
	m_sprites.clear();
	//Loop through all textures and delete them
	auto itTexture = m_textures.begin();
	while (itTexture != m_textures.end())
	{
		delete itTexture->second;
		++itTexture;
	}
	m_textures.clear();


	//Loop through all texts and delete them
	auto itText = m_texts.begin();
	while (itText != m_texts.end())
	{
		delete (*itText);
		++itText;
	}
	m_texts.clear();
	//Loop through all textures and delete them
	auto itFont = m_fonts.begin();
	while (itFont != m_fonts.end())
	{
		delete itFont->second;
		++itFont;
	}
	m_fonts.clear();
}

sf::Sprite* SpriteManager::CreateSprite(const std::string& filename)
{
	//Check if the wanted texture already exists
	auto it = m_textures.find(filename);

	if (it == m_textures.end())
	{
		//If it does not exist, create a new Texture and load the file
		sf::Texture* texture = new sf::Texture();
		texture->loadFromFile(filename);

		//Insert the texture into a map so that the next time we create a texture it will find it at the beginning of the function. 
		//We also insert it so that we can delete them all in the destructor.
		m_textures.insert(std::pair<std::string, sf::Texture*>(filename, texture));

		//Set the iterator to the new texture so we can return it later
		it = m_textures.find(filename);
	}


	//Create a sprite from the found/created texture
	sf::Sprite* sprite = new sf::Sprite(*it->second);

	//Insert the sprite into a vector so we can delete all sprites in the destructor.
	m_sprites.push_back(sprite);


	//Return the newly created sprite
	return sprite;
}
sf::Sprite* SpriteManager::CreateSprite(const std::string& filename, sf::IntRect sourceRect)
{
	//Check if the wanted texture already exists
	auto it = m_textures.find(filename);

	if (it == m_textures.end())
	{
		//If it does not exist, create a new Texture and load the file
		sf::Texture* texture = new sf::Texture();
		texture->loadFromFile(filename);

		//Insert the texture into a map with extension so that the next time we create a texture it will find it at the beginning of the function. 
		//We also insert it so that we can delete them all in the destructor.
		m_textures.insert(std::pair<std::string, sf::Texture*>(filename, texture));

		//Set the iterator to the new texture so we can return it later
		it = m_textures.find(filename);
	}

	//Create a sprite from the found/created texture with an asigned sourceRectangle
	sf::Sprite* sprite = new sf::Sprite(*it->second, sourceRect);

	//Insert the sprite into a vector so we can delete all sprites in the destructor.
	m_sprites.push_back(sprite);

	return sprite;
}

sf::Text* SpriteManager::CreateText(const std::string& font_filename, const std::string& text, unsigned int charSize)
{
	std::string extension = "_" + std::to_string(charSize);
	auto it = m_fonts.find(font_filename + extension);

	if (it == m_fonts.end())
	{
		sf::Font* font = new sf::Font();
		font->loadFromFile(font_filename);
		
		m_fonts.insert(std::pair<std::string, sf::Font*>(font_filename + extension, font));
		it = m_fonts.find(font_filename + extension);
	}

	sf::Text* sfText = new sf::Text(text, *it->second, charSize);
	m_texts.push_back(sfText);

	return sfText;
}