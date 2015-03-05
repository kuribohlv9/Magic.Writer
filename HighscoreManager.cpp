#include "stdafx.h"
#include "HighscoreManager.h"
#include "DrawManager.h"

HighscoreManager::HighscoreManager()
{
	m_highscore_filename = "assets/highscore.txt";
	m_maxEntries = 10;

	//Read all highscores
	ReadHighscore();
}
HighscoreManager::~HighscoreManager()
{
}

bool HighscoreManager::Initialize()
{
	return true;
}

void HighscoreManager::Draw(DrawManager* drawManager)
{
	for (int i = 0; i < m_entries.size(); i++)
	{
		if (i < m_maxEntries)
		{
			m_text.setString(std::to_string(i + 1) + ". " + m_entries[i].GetText());
			m_text.setPosition(m_position.x, m_position.y + i * 45);

			drawManager->Draw(m_text, sf::RenderStates::Default);

		}
		else if (m_lastEntry.name == m_entries[i].name)
		{
			m_text.setString(std::to_string(i + 1) + ". " + m_entries[i].GetText());
			m_text.setPosition(m_position.x, m_position.y + (m_maxEntries + 1) * 45);

			drawManager->Draw(m_text, sf::RenderStates::Default);
		}
	}
}
void HighscoreManager::WriteHighscore(ScoreEntry entry)
{
	m_lastEntry = entry;

	std::ofstream file;
	file.open(m_highscore_filename);

	bool entryApplied = false;
	for (int i = 0; i < m_entries.size(); i++)
	{
		if (entry.score > m_entries[i].score && !entryApplied)
		{
			m_entries.insert(m_entries.begin() + i, entry);
			entryApplied = true;
		}

		file << m_entries[i].name;
		file << " " << m_entries[i].score;

		if (i < m_entries.size() - 1)
			file << std::endl;
	}

	if (!entryApplied)
	{
		m_entries.push_back(entry);
		file << std::endl << m_entries[m_entries.size() - 1].name;
		file << " " << m_entries[m_entries.size() - 1].score;
	}

	file.close();
}
void HighscoreManager::ReadHighscore()
{
	m_entries.clear();

	std::ifstream file;
	file.open(m_highscore_filename);

	while (!file.eof())
	{
		ScoreEntry entry;
		file >> entry.name;
		file >> entry.score;

		m_entries.push_back(entry);
	}

	file.close();
}
void HighscoreManager::SetFont(sf::Font* font)
{
	m_text.setFont(*font);
	m_text.setCharacterSize(40);
}
void HighscoreManager::SetPosition(float x, float y)
{
	m_position = sf::Vector2f(x, y);
}
