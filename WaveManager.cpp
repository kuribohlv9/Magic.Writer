#include "stdafx.h"
#include "WaveManager.h"
#include <fstream>
#include <iostream>

WaveManager::WaveManager()
{
	LoadWave("assets/waves/waves.txt");
	m_spawnTime = m_waves[0].at(0);

	m_active = true;
}

void WaveManager::Update(float deltaTime)
{
	if (!m_active)
		return;

	m_timer += deltaTime;

	if (m_timer >= m_spawnTime)
	{
		std::cout << "Spawn" << std::endl;
		//Get the current wave

		std::vector<float>* currentWave = &m_waves[0];

		if (currentWave->size() > 0)
		{
			m_spawnTime = currentWave->at(0);
			currentWave->erase(currentWave->begin());
			m_spawnMonster = true;
			m_timer = 0;
		}
		
		if (currentWave->size() == 0)
		{
			//Remove finished wave
			m_waves.erase(m_waves.begin());

			if (m_waves.size() <= 0)
			{
				m_active = false;
				return;
			}

			m_spawnTime = currentWave->at(0);
			m_timer = 0;
		}
	}

}
bool WaveManager::CanSpawnMonster()
{
	if (m_spawnMonster)
	{
		m_spawnMonster = false;
		return true;
	}

	return false;
}
void WaveManager::LoadWave(const std::string& filename)
{
	std::ifstream stream;
	stream.open(filename);

	std::vector<float> waveTimes;
	while (!stream.eof())
	{
		//Get current line in .txt
		std::string line;
		stream >> line;


		//Check if the line is a new wave
		if (line == "wave")
		{
			if (waveTimes.size() != 0)
			{
				//Add the last wave and clear to get ready for a new wave
				m_waves.push_back(waveTimes);
				waveTimes.clear();
			}
		}
		else
		{
			//Enter the timestamp for monsterspawning
			waveTimes.push_back(std::stof(line));
		}
	}

	//Add the last wave
	m_waves.push_back(waveTimes);
}