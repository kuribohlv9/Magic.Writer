#pragma once

class DrawManager;

struct ScoreEntry
{
	//Stores the users name
	std::string name;

	//Stores the users score
	int score;

	//Returns a string that connects the name and score as one string
	std::string GetText()
	{
		if (name == "PUMZEEE" || name == "TUNA" || name == "ARA" || name == "KURI" || name == "SCHULLE" || name == "FIRENZ") //Add more developer names here with ||...
		{
			return "(D)" + name + " " + std::to_string(score);
		}
		else
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

	unsigned int m_maxEntries;
	ScoreEntry m_lastEntry;
	sf::Vector2f m_position;
	sf::Text m_text;
};

