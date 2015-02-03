#pragma once

class DrawManager;
class TextureManager;
class InputManager;

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
	std::string m_words[3];
	bool m_words_active[3];
};