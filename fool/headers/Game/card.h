#pragma once

#include "Engine/SceneNode.h"
#include "Engine/Resource/ResourceIdentifiers.h"
#include "Engine/Resource/ResourceHolder.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Audio/Sound.hpp>

#include <string>
#include <memory>

class Card : public SceneNode
{
public:
	typedef std::unique_ptr<Card> Ptr;
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
	explicit Card(Suit suit, Rank rank, const TextureHolder &textures, SoundHolder &sounds);

	Rank getRank() const;
	Suit getSuit() const;

	// change side of the card
	// for example from reverse to face and back
	void changeSide(bool isFaceSide);

	virtual void handleEvent(const sf::Event &event);
	void checkSelection(sf::Vector2f mousePosition);
	sf::FloatRect getGlobalBounds() const;

	bool isSelected() const;
	void select();
	void deselect();

	bool operator<(const Card &compareTo);
	bool operator>(const Card &compareTo);
	bool operator==(const Card &compareTo);
	bool operator<=(const Card &compareTo);
	bool operator>=(const Card &compareTo);

private:
	virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	Rank mRank;
	Suit mSuit;
	bool misFaceSide;
	bool mIsSelected;

	sf::Sound mSelectedSound;

	sf::Sprite mFrontFaceSprite;
	sf::Sprite mReverseFaceSprite;
};