#pragma once

class SpriteManager
{
public:
	SpriteManager();
	~SpriteManager();

	//Creates and returns a sprite via a filename
	sf::Sprite* CreateSprite(const std::string& filename);
	//Creates and returns a sprite via a filename and a sourceRectangle
	sf::Sprite* CreateSprite(const std::string& filename, sf::IntRect sourceRect);

	//Creates and returns a text via filename, text and a font size
	sf::Text* CreateText(const std::string& font_filename, const std::string& text, unsigned int charSize);

private:
	//Saves pointers to textures and sprites
	std::vector<sf::Sprite*> m_sprites;
	std::map<std::string, sf::Texture*> m_textures;

	//Saves pointers to fonts and texts
	std::vector<sf::Text*> m_texts;
	std::map<std::string, sf::Font*> m_fonts;
};