#include "GameState.h"
#include "Utility.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <cmath>

GameState::GameState(StateStack &stack, Context context)
	: State(stack, context)
{
	time = sf::Time::Zero;
	for (Card::Suit suit = Card::Diamonds; suit <= Card::Spades; suit = Card::Suit((int)suit + 1))
	{
		for (Card::Rank rank = Card::_2; rank <= Card::_Ace; rank = Card::Rank((int)rank + 1))
		{
			mTextures.load(Textures::ID((Textures::Global::ID)suit, (Textures::Local::ID)rank), getFilePath(suit, rank));
		}
	}

	mTextures.load(Textures::ID(Textures::Global::ReverseFace), getFilePath(Textures::Global::ReverseFace));

	for (Card::Suit suit = Card::Diamonds; suit <= Card::Spades; suit = Card::Suit((int)suit + 1))
	{
		std::vector<Card> temp;
		for (Card::Rank rank = Card::_2; rank <= Card::_Ace; rank = Card::Rank((int)rank + 1))
		{
			temp.push_back(Card(suit, rank, mTextures));
		}
		mCards.push_back(temp);
	}

	for (int i = 0; i < mCards.size(); i++)
	{
		for (int j = 0; j < mCards[i].size(); j++)
		{
			mCards[i][j].setScale(120.f / 165.f, 120.f / 165.f);
			mCards[i][j].setPosition(120.f * j, 170.f * i);
		}
	}
}

void GameState::draw()
{
	sf::RenderWindow &window = *getContext().window;

	for (int i = 0; i < mCards.size(); i++)
	{
		for (int j = 0; j < mCards[i].size(); j++)
		{
			window.draw(mCards[i][j]);
		}
	}
}

bool GameState::update(sf::Time dt)
{
	for (int i = 0; i < mCards.size(); i++)
	{
		for (int j = 0; j < mCards[i].size(); j++)
		{
			time += dt;
			mCards[i][j].setScale(abs(cos(time.asSeconds() / (6 * 3.14159))) * 120.f / 165.f, 120.f / 165.f);
		}
	}

	return true;
}

bool GameState::handleEvent(const sf::Event &event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			requestStackPush(States::Pause);
			break;
		}
	}
	return true;
}
