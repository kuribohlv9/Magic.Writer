#pragma once

class Monster;

class WaveManager
{
public:
	WaveManager();

	void Update(float deltaTime);
	bool CanSpawnMonster();
	void Reset();
	void SetActive(bool state);
	bool IsActive();

private:
	void LoadWave(const std::string& filename);

private:
	std::vector<std::vector<float>> m_waves;
	int m_waveTime;
	int m_monster_number;
	float m_timer;
	float m_spawnTime;
	bool m_spawnMonster;
	bool m_active;
};