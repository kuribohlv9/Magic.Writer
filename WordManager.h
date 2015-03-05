#pragma once

class DrawManager;
class TextureManager;
class InputManager;

struct Word
{
	std::string text;
	bool active;
	sf::Vector2f position;
};

class WordManager
{
public:
	WordManager();

	void Update(float deltaTime);
	void Draw(DrawManager* drawManager);

	//Returns any any finished word
	std::string GetFinishedWord();

	//Sets a new word
	void SetNewWord(const std::string& newWord);
	void SetWordPosition(sf::Vector2f position, int wordIndex);

	bool GetCorrectKey();
	std::vector<bool> GetActiveBubbles();

private:
	TextureManager* m_textureManager;
	InputManager* m_inputManager;

	//Checks if keypress matches words
	void CheckWords();

	//Reset the words active state and users input
	void Reset();

	//Text to draw the words
	sf::Text m_text;

	//The users full input and the current key
	std::string m_userInput;
	char m_userChar;

	//The current words text and active states
	std::vector<Word> m_structs;
	bool m_correctKey;
};