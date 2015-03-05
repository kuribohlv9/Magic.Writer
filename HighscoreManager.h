#pragma once

class DrawManager;

struct ScoreEntry
{
	std::string name;
	int score;

	std::string GetText()
	{
		return (name + " " + std::to_string(score));
	}
};

class HighscoreManager
{
public:
	HighscoreManager();
	~HighscoreManager();

	bool Initialize();

	void Draw(DrawManager* drawManager);

	void SetPosition(float x, float y);
	void SetFont(sf::Font* font);
	void WriteHighscore(ScoreEntry score);

private:
	void ReadHighscore();

private:
	std::vector<ScoreEntry> m_entries;
	std::string m_highscore_filename;

	int m_maxEntries;
	ScoreEntry m_lastEntry;
	sf::Vector2f m_position;
	sf::Text m_text;
};

