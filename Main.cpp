// Main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Word.h"
#include <iostream>

int main(int argc, char* argv[])
{
	sf::RenderWindow window;
	window.create(sf::VideoMode(1024, 600), "Magic Writer");

	char inputChar;
	bool enter;
	bool left, lastLeft, right, lastRight;
	int xPositionIndex = 1;
	std::string inputText;

	//Instantiate player
	sf::Texture texture;
	texture.loadFromFile("assets/sprites/object.png");

	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setPosition(380, 300);

	//Instantiate words
	Word* words[3];
	words[0] = new Word("brick", sf::Vector2f(200, 400));
	words[1] = new Word("ball", sf::Vector2f(400, 400));
	words[2] = new Word("chair", sf::Vector2f(600, 400));
	int wordCount = sizeof(words) / sizeof(words[0]);

	std::vector<sf::Text> finishedWords;

	//Load font
	sf::Font font;
	font.loadFromFile("assets/fonts/font.ttf");

	sf::Text userText;
	userText.setFont(font);
	userText.setPosition(400, 450);

	sf::Text singleCharText;
	singleCharText.setFont(font);

	//Game loop
	while (window.isOpen())
	{
		sf::Event event;

		enter = false;
		while (window.pollEvent(event))
		{
			//Check event types
			//Close window with X
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode < 1000)
				{
					inputChar = static_cast<char>(event.text.unicode);
					if (inputChar == '\r')
					{
						enter = true;
						continue;
					}
					else if (inputChar == '\b')
					{
						inputText = "";
						userText.setString(inputText);

						for (int i = 0; i < wordCount; i++)
						{
							words[i]->SetActive(true);
						}
						continue;
					}

					inputText += inputChar;
					userText.setString(inputText);
				}
			}
		}

		//Move sprite
		left = false;
		right = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			left = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			right = true;
		}

		if (left && !lastLeft)
		{
			if (xPositionIndex > 1)
				xPositionIndex--;
		}
		else if (right && !lastRight)
		{
			if (xPositionIndex < 3)
				xPositionIndex++;
		}

		sprite.setPosition(10 + xPositionIndex * 190, 200);

		//Clear screen with single color
		window.clear(sf::Color::Black);

		//Draw stuff here

		//Draw words
		window.draw(userText);
		for (int i = 0; i < wordCount; i++)
		{
			std::string wordText = words[i]->GetText();
			sf::Text text;

			text.setFont(font);
			text.setString(wordText);
			text.setPosition(words[i]->GetPosition());
			
			int charIndex = -1;
			if (words[i]->IsActive() && inputText.size() > 0)
			{
				charIndex = inputText.size() - 1;
				
				if (charIndex >= wordText.size())
					continue;

				if (wordText.size() == inputText.size() && enter)
				{
					sf::Text finishedText;
					finishedText.setFont(font);
					finishedText.setString(wordText);
					finishedText.setPosition(sprite.getPosition());

					finishedWords.push_back(finishedText);

					//Reset... the same as backslash
					inputText = "";
					userText.setString(inputText);
					for (int i = 0; i < wordCount; i++)
					{
						words[i]->SetActive(true);
					}

				}
				else if (wordText[charIndex] != inputChar && inputChar != '\r')
				{
					words[i]->SetActive(false);
					continue;
				}
				

			}
		
			if (charIndex >= 0)
			{
				for (int j = 0; j < wordText.size(); j++)
				{
					sf::Vector2f position = words[i]->GetPosition();

					singleCharText.setColor(sf::Color::White);
					if (j <= charIndex)
						singleCharText.setColor(sf::Color::Red);
					singleCharText.setString(wordText[j]);
					singleCharText.setPosition(position.x + j * 15, position.y);

					window.draw(singleCharText);
				}
			}
			else
			{
				window.draw(text);
			}
		}

		//Draw all finished words
		for (int i = 0; i < finishedWords.size(); i++)
		{
			
			finishedWords[i].move(0, -1);

			window.draw(finishedWords[i]);
		}

		window.draw(sprite);
		//

		//Present back buffer
		window.display();

		lastLeft = left;
		lastRight = right;
	}
	return 0;
}

