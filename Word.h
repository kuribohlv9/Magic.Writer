#pragma once
class Word
{
public:
	Word(std::string word, sf::Vector2f position);
	~Word();

	sf::Vector2f GetPosition();
	std::string GetText();
	bool IsActive();
	void SetActive(bool state);

private:
	sf::Vector2f m_position;
	std::string m_text;
	bool m_active;
};

