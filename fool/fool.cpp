#include "Card.h"
#include "Utility.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

int main()
{
	try
	{
		TextureHolder textures;
		for (Card::Suit suit = Card::Diamonds; suit <= Card::Spades; suit = Card::Suit((int)suit + 1))
		{
			for (Card::Rank rank = Card::_2; rank <= Card::_Ace; rank = Card::Rank((int)rank + 1))
			{
				textures.load(Textures::ID((Textures::Global::ID)suit, (Textures::Local::ID)rank), getFilePath(suit, rank));
			}
		}

		std::vector<std::vector<Card>> cards;

		for (Card::Suit suit = Card::Diamonds; suit <= Card::Spades; suit = Card::Suit((int)suit + 1))
		{
			std::vector<Card> temp;
			for (Card::Rank rank = Card::_2; rank <= Card::_Ace; rank = Card::Rank((int)rank + 1))
			{
				temp.push_back(Card(suit, rank, textures));
			}
			cards.push_back(temp);
		}

		for (int i = 0; i < cards.size(); i++)
		{
			for (int j = 0; j < cards[i].size(); j++)
			{
				cards[i][j].setScale(120.f / 165.f, 120.f / 165.f);
				cards[i][j].setPosition(120.f * j, 170.f * i);
			}
		}

		sf::RenderWindow window(sf::VideoMode(1600, 900), "FUCK YOU");
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

			window.clear();
			for (int i = 0; i < cards.size(); i++)
			{
				for (int j = 0; j < cards[i].size(); j++)
				{
					window.draw(cards[i][j]);
				}
			}
			window.display();
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}
