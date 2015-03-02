#pragma once
class HighscoreManager
{
public:
	HighscoreManager();
	~HighscoreManager();

	bool Initialize();

	//Used in function to sort the highscorelist
	

	void WriteHighscore(int score);
	void ReadHighscore();
	std::vector<int>* GetHighscore();

private:
	std::vector<int>* m_highscore;
	//mysortpls derp;
};

