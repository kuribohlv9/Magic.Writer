#pragma once

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	//Creates and returns a texture via a filename
	sf::Texture* LoadTexture(const std::string& filename);
	//Creates and returns a font via filename
	sf::Font* LoadFont(const std::string& font_filename);

private:
	//Saves pointers to textures
	std::map<std::string, sf::Texture*> m_textures;

	//Saves pointers to fonts
	std::map<std::string, sf::Font*> m_fonts;
};