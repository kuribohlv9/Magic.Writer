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
	sf::Font* font = m_textureManager->LoadFont("assets/fonts/font.ttf");
	m_text.setFont(*font);

	//TMP, SET WORDS
	m_words[0] = "panda";
	m_words[1] = "bread";
	m_words[2] = "pork";

	//Reset all words
	Reset();
}

void WordManager::Update(float deltaTime)
{
	//Get input key from keyboard
	m_userChar = m_inputManager->GetInputChar();

	//Check if a key was pressed
	if (m_userChar != ' ')
	{
		//If a key was pressed, check the words using the character
		CheckWords();
	}

	//TMP
	if (m_inputManager->IsKeyDownOnce(sf::Keyboard::Key::BackSpace))
		Reset();
}
void WordManager::Draw(DrawManager* drawManager)
{
	//Loop through each word
	for (int i = 0; i < 3; i++)
	{
		//Set a position for the word
		sf::Vector2f position = sf::Vector2f(200.0f + 100.0f * i, 500.0f);

		//Get the current word and it's active state
		std::string word = m_words[i];
		bool active = m_words_active[i];

		//Apply the position and color to m_text
		m_text.setPosition(position);
		m_text.setColor(sf::Color(25, 25, 25, 255));

		//Check if the current word is active
		if (active)
		{
			//For each active word we loop through each character in the word
			for (int j = 0; j < word.size(); j++)
			{
				//We set the characters position and color
				sf::Vector2f localPosition = sf::Vector2f(position.x + j * 15, position.y);
				sf::Color color = sf::Color::White;

				//Gets the current character
				char currentChar = word[j];

				//Apply the character to m_text
				m_text.setString(currentChar);

				//Check if the current characters index is less than the users word input. If so apply a green color
				if (j < m_userInput.size())
				{
					color = sf::Color::Green;
				}

				//Apply position and color (green or gray);
				m_text.setColor(color);
				m_text.setPosition(localPosition);

				//Draw the current character
				drawManager->Draw(m_text, sf::RenderStates::Default);
			}
		}
		else
		{
			//If the word is not active, apply the word to m_text and draw it onto the screen
			m_text.setString(word);
			drawManager->Draw(m_text, sf::RenderStates::Default);
		}
	}
}
void WordManager::CheckWords()
{
	//A boolean for checking if the pressed key(userChar) fits the word.
	bool charFits = false;

	//Loop through all words
	for (int i = 0; i < 3; i++)
	{
		//Get the current word and it's active state
		std::string word = m_words[i];
		bool active = m_words_active[i];


		//If the word is active and the current character fits the words length
		if (active && m_userInput.size() < word.size())
		{
			//Checks if the key does not match the word, set the word to inactive
			if (word[m_userInput.size()] != m_userChar)
			{
				m_words_active[i] = false;
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
		m_userInput += m_userChar;
	}


	//Loop through the words again
	for (int i = 0; i < 3; i++)
	{
		//Get the current word
		std::string word = m_words[i];

		//If the users word 
		if (m_userInput.size() >= word.size())
			continue;

		//Cut the current word into the same length as the users input
		std::string cutWord = word;
		cutWord.erase(cutWord.begin() + m_userInput.size(), cutWord.end());

		//If the cutword matches the users input the word is activated again
		if (cutWord == m_userInput)
		{
			m_words_active[i] = true;
		}
	}
}

void WordManager::Reset()
{
	//Sets all words to active
	for (int i = 0; i < 3; i++)
	{
		m_words_active[i] = true;
	}

	//Reset the userinput and character
	m_userInput = "";
	m_userChar = ' ';
}

std::string WordManager::GetFinishedWord()
{
	std::string finishedWord = "";

	//Find any word that matches the user input and return the word. Set the word slot to empty string.
	for (int i = 0; i < 3; i++)
	{
		if (m_words[i] == m_userInput)
		{
			finishedWord = m_words[i];
			m_words[i] = "";
			Reset();
			break;
		}
	}

	return finishedWord;
}
void WordManager::SetNewWord(const std::string& newWord)
{
	//Apply the newWord to empty word slot
	for (int i = 0; i < 3; i++)
	{
		if (m_words[i] == "")
		{
			m_words[i] = newWord;
			break;
		}
	}
}