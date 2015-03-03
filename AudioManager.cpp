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
	auto itrs = m_buffers.begin();
	while (itrs != m_buffers.end())
	{
		delete itrs->second;
		++itrs;
	}
	m_buffers.clear();

	auto itrm = m_musics.begin();
	while (itrm != m_musics.end())
	{
		delete itrm->second;
		++itrm;
	}
	m_musics.clear();
}

sf::SoundBuffer* AudioManager::LoadSoundFromFile(const std::string& filename)
{
	auto it = m_buffers.find(filename);

	if (it == m_buffers.end())
	{
		sf::SoundBuffer* buffer = new sf::SoundBuffer();
		buffer->loadFromFile(filename);

		m_buffers.insert(std::pair<std::string, sf::SoundBuffer*>(filename, buffer));

		it = m_buffers.find(filename);
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