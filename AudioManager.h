class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	bool Initialize();
	void Shutdown();

	sf::Sound* LoadSoundFromFile(const std::string& filename);
	sf::Music* LoadMusicFromFile(const std::string& filename);

private:
	std::map<std::string, sf::Sound*> m_sounds;
	std::map<std::string, sf::Music*> m_musics;
};