#pragma once

#include "SceneNode.h"
#include "ResourceIdentifiers.h"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include <string>

class Card : public SceneNode
{
public:
	enum Rank
	{
		_2 = 2,
		_3,
		_4,
		_5,
		_6,
		_7,
		_8,
		_9,
		_10,
		_Jack,
		_Queen,
		_King,
		_Ace
	};

	enum Suit
	{
		Diamonds = 1,
		Hearts = 2,
		Clubs = 3,
		Spades = 4
	};

public:
	explicit Card(Suit suit, Rank rank, const TextureHolder &textures);

	Rank getRank();
	Suit getSuit();

	// change side of the card
	// for example from reverse to face and back
	// void changeSide();

	virtual void handleEvent(const sf::Event &event);

private:
	virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	Rank mRank;
	Suit mSuit;

	sf::Sprite mFrontFaceSprite;
	sf::Sprite mReverseFaceSprite;
};