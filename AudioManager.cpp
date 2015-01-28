#include "stdafx.h"
#include "AudioManager.h"

AudioManager::AudioManager()
{
}
AudioManager::~AudioManager()
{
}

bool AudioManager::Initialize()
{
	return true;
}

void AudioManager::Shutdown()
{
	//Deletes all loaded sounds
	auto itrs = m_sounds.begin();
	while (itrs != m_sounds.end())
	{
		delete itrs->second;
		++itrs;
	}
	m_sounds.clear();

	auto itrm = m_musics.begin();
	while (itrm != m_musics.end())
	{
		delete itrm->second;
		++itrm;
	}
	m_musics.clear();
}

sf::Sound* AudioManager::LoadSoundFromFile(const std::string& filename)
{
	auto it = m_sounds.find(filename);

	if (it == m_sounds.end())
	{
		sf::SoundBuffer buffer;
		buffer.loadFromFile(filename);

		sf::Sound* sound = new sf::Sound(buffer);
		m_sounds.insert(std::pair<std::string, sf::Sound*>(filename, sound));

		it = m_sounds.find(filename);
	}

	return it->second;
}
sf::Music* AudioManager::LoadMusicFromFile(const std::string& filename)
{
	auto it = m_musics.find(filename);

	if (it == m_musics.end())
	{
		sf::Music* music = new sf::Music();
		music->openFromFile(filename);
		m_musics.insert(std::pair<std::string, sf::Music*>(filename, music));

		it = m_musics.find(filename);
	}

	return it->second;
}