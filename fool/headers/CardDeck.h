#pragma once

#include "SceneNode.h"
#include "Card.h"

#include <vector>

class CardDeck : public SceneNode
{
public:
	typedef std::unique_ptr<CardDeck> Ptr;

public:
	CardDeck();

	virtual void handleEvent(const sf::Event &event);

	void pushCard(Card::Ptr card);
	Card::Ptr popCard();
	Card::Suit getTrump();
	bool isEmpty() const;

	void shuffle();

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	std::vector<Card *> mCards;
	Card::Suit mTrump;
};
