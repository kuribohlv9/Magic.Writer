#include "stdafx.h"
#include "WordManager.h"
#include "DrawManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "ServiceLocator.h"

WordManager::WordManager()
{
	//Get texturemanager and inputmanager
	m_textureManager = ServiceLocator<TextureManager>::GetService();
	m_inputManager = ServiceLocator<InputManager>::GetService();

	//Load a font
	sf::Font* font = m_textureManager->LoadFont("assets/fonts/game.ttf");
	m_text.setFont(*font);
	m_text.setCharacterSize(23);

	//Create words
	for (int i = 0; i < 3; i++)
	{
		Word w;
		w.text = "";
		w.position = sf::Vector2f(0, 0);
		w.active = true;

		m_structs.push_back(w);
	}

	//Reset all words
	Reset();
}

void WordManager::Update(float deltaTime)
{
	for (size_t i = 0; i < m_structs.size(); i++)
	{
		if (m_structs[i].text == "")
		{
			return;
		}
	}

	//Get input key from keyboard
	m_correctKey = false;
	m_userChar = m_inputManager->GetInputChar();

	//Check if a key was pressed
	if (m_userChar != ' ')
	{
		if (m_userInput.size() == 0)
		{
			m_perfectWord = true;
		}
		//If a key was pressed, check the words using the character
		CheckWords();
	}
}
void WordManager::Draw(DrawManager* drawManager)
{
	//Loop through each word
	for (size_t i = 0; i < m_structs.size(); i++)
	{
		std::string word = m_structs[i].text;
		m_text.setString(word);
		m_text.setColor(sf::Color(137, 177, 185, 255)); // Light
		m_text.setPosition(m_structs[i].position);

		int wordWidth = (int)m_text.getGlobalBounds().width;
		m_text.move(-wordWidth / 2.0f + 5.0f, 20.0f);

		if (m_structs[i].active)
		{
			m_text.setColor(sf::Color(30, 58, 64, 255)); // Dark
			drawManager->Draw(m_text, sf::RenderStates::Default);
			if (m_userInput.size() > 0)
			{
				int user = m_userInput.size();
				int wordSize = word.size();
				std::string subWord = word;

				subWord.erase(m_userInput.size(), wordSize - user);
				m_text.setColor(sf::Color(88, 153, 11, 255)); // Green
				m_text.setString(subWord);
				drawManager->Draw(m_text, sf::RenderStates::Default);
			}
		}
		else
		{
			drawManager->Draw(m_text, sf::RenderStates::Default);
		}
	}
}
void WordManager::CheckWords()
{
	//A boolean for checking if the pressed key(userChar) fits the word.
	bool charFits = false;

	//Loop through all words
	for (size_t i = 0; i < m_structs.size(); i++)
	{
		//Get the current word and it's active state
		std::string word = m_structs[i].text;
		bool active = m_structs[i].active;


		//If the word is active and the current character fits the words length
		if (active && m_userInput.size() < word.size())
		{
			//Checks if the key does not match the word, set the word to inactive
			if (word[m_userInput.size()] != m_userChar)
			{
				m_structs[i].active = false;
			}
			else
			{
				//If the key fits, tagg the boolean as true
				charFits = true;
			}
		}
	}

	//If the key fits in any word, apply the key to the full userText
	if (charFits)
	{
		m_correctKey = true;
		m_userInput += m_userChar;
	}
	else
	{
		if (m_userInput.size() > 0)
		{
			m_perfectWord = false;
		}
	}


	//Loop through the words again
	for (size_t i = 0; i < m_structs.size(); i++)
	{
		//Get the current word
		std::string word = m_structs[i].text;

		//If the users word 
		if (m_userInput.size() >= word.size())
			continue;

		//Cut the current word into the same length as the users input
		std::string cutWord = word;
		cutWord.erase(cutWord.begin() + m_userInput.size(), cutWord.end());

		//If the cutword matches the users input the word is activated again
		if (cutWord == m_userInput)
		{
			m_structs[i].active = true;
		}
	}
}

void WordManager::Reset()
{
	//Sets all words to active
	for (size_t i = 0; i < m_structs.size(); i++)
	{
		m_structs[i].active = true;
	}

	//Reset the userinput and character
	m_userInput = "";
	m_userChar = ' ';
}
void WordManager::SetWordPosition(sf::Vector2f position, int wordIndex)
{
	m_structs[wordIndex].position = position;
}

std::string WordManager::GetFinishedWord(bool& perfectWord)
{
	std::string finishedWord = "";
	perfectWord = false;

	//Find any word that matches the user input and return the word. Set the word slot to empty string.
	for (size_t i = 0; i < m_structs.size(); i++)
	{
		if (m_structs[i].text == m_userInput)
		{
			finishedWord = m_structs[i].text;
			m_structs[i].text = "";
			Reset();
			m_correctKey = false;
			perfectWord = m_perfectWord;
			m_perfectWord = false;
			return finishedWord;
		}
	}

	return "";
}
void WordManager::SetNewWord(const std::string& newWord)
{
	//Apply the newWord to empty word slot
	for (size_t i = 0; i < m_structs.size(); i++)
	{
		if (m_structs[i].text.size() == 0)
		{
			m_structs[i].text = newWord;
			break;
		}
	}
}

std::vector<bool> WordManager::GetActiveBubbles()
{
	std::vector<bool> results;

	results.push_back(m_structs[0].active);
	results.push_back(m_structs[1].active);
	results.push_back(m_structs[2].active);
	return results;
}
bool WordManager::GetCorrectKey()
{
	return m_correctKey;
}