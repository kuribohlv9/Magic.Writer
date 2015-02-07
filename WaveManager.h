#pragma once

class WaveManager
{
public:
	WaveManager();

private:
	void LoadWave(const std::string& filename);

private:
	std::vector<std::vector<float>> m_waves;
};