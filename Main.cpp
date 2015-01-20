// Main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main(int argc, char* argv[])
{
	sf::Window window;
	window.create(sf::VideoMode(1024, 600), "Magic Writer");

	//Game loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			//Check event types



			//Close window with X
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

	}
	return 0;
}

