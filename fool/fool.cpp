#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <windows.h>
#include "card.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(400, 300), "FUCK YOU");
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
	}
	return 0;
}
