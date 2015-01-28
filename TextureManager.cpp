#include "stdafx.h"
#include "TextureManager.h"

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{
}

bool TextureManager::Initialize()
{
	return true;
}
void TextureManager::Shutdown()
{
	//Loop through all textures and delete them
	auto itTexture = m_textures.begin();
	while (itTexture != m_textures.end())
	{
		delete itTexture->second;
		++itTexture;
	}
	m_textures.clear();

	//Loop through all fonts and delete them
	auto itFont = m_fonts.begin();
	while (itFont != m_fonts.end())
	{
		delete itFont->second;
		++itFont;
	}
	m_fonts.clear();
}
sf::Texture* TextureManager::LoadTexture(const std::string& filename)
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

	//Return the newly created sprite
	return it->second;
}
sf::Font* TextureManager::LoadFont(const std::string& filename)
{
	//Check if the wanted texture already exists
	auto it = m_fonts.find(filename);

	if (it == m_fonts.end())
	{
		//If it does not exist, create a new Texture and load the file
		sf::Font* font = new sf::Font();
		font->loadFromFile(filename);

		//Insert the texture into a map so that the next time we create a texture it will find it at the beginning of the function. 
		//We also insert it so that we can delete them all in the destructor.
		m_fonts.insert(std::pair<std::string, sf::Font*>(filename, font));

		//Set the iterator to the new texture so we can return it later
		it = m_fonts.find(filename);
	}

	return it->second;
}