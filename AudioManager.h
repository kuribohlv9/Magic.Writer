class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	bool Initialize();
	void Shutdown();

	sf::SoundBuffer* LoadSoundFromFile(const std::string& filename);
	sf::Music* LoadMusicFromFile(const std::string& filename);

private:
	std::map<std::string, sf::SoundBuffer*> m_buffers;
	std::map<std::string, sf::Music*> m_musics;
};