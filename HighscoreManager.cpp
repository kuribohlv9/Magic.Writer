#include "stdafx.h"
#include "HighscoreManager.h"


HighscoreManager::HighscoreManager()
{
	m_highscore = new std::vector < int > ;
	ReadHighscore();
}


HighscoreManager::~HighscoreManager()
{
}

bool HighscoreManager::Initialize()
{
	return true;
}

void HighscoreManager::WriteHighscore(int score)
{
	m_highscore->push_back(score);

	//Reason why this works http://en.cppreference.com/w/cpp/algorithm/sort
	std::sort(m_highscore->begin(), m_highscore->end(), [](int a, int b) {return b < a; });

	std::ofstream file;
	file.open("assets/highscore.txt");
	auto itr = m_highscore->begin();
	while (itr != m_highscore->end())
	{
		file << std::to_string(*itr);
		file << "\n";
		itr++;
	}

	file.close();
}

void HighscoreManager::ReadHighscore()
{
	std::ifstream file;
	file.open("assets/highscore.txt");

	std::string text;
	while (!file.eof())
	{
		file >> text;
		m_highscore->push_back(std::stoi(text, nullptr));
	}

	file.close();

	//Reason why this works http://en.cppreference.com/w/cpp/algorithm/sort
	std::sort(m_highscore->begin(), m_highscore->end(), [](int a, int b) {return b < a;});
}

std::vector<int>* HighscoreManager::GetHighscore()
{
	return m_highscore;
}
