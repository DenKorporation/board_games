#include "CardDeck.h"
#include "SpriteNode.h"

#include <algorithm>
#include <random>
#include <stdexcept>

CardDeck::CardDeck()
	: mCards(),
	  mTrump(Card::Suit::Diamonds)
{
}

void CardDeck::handleEvent(const sf::Event &event)
{
}

void CardDeck::pushCard(Card::Ptr card)
{
	mCards.push_back(card.get());
	attachChild(std::move(card));
}

Card::Ptr CardDeck::popCard()
{
	if (mCards.size() == 0)
	{
		throw std::runtime_error("CardDeck::popCard unable to popCard from empty std::vector");
	}

	Card::Ptr result(static_cast<Card *>(detachChild((*mCards[mCards.size() - 1])).release()));
	mCards.pop_back();

	return std::move(result);
}

Card::Suit CardDeck::getTrump()
{
	return mTrump;
}

void CardDeck::shuffle()
{
	auto rd = std::random_device{};
	auto rng = std::default_random_engine{rd()};
	std::shuffle(std::begin(mCards), std::end(mCards), rng);
	for (Card *child : mCards)
	{
		child->changeSide(false);
	}
	mCards[0]->changeSide(true);
}

void CardDeck::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	if (mCards.size() == 0)
	{
		drawChildren(target, states);
	}
	else
	{
		sf::RenderStates customStates = states;
		customStates.transform.rotate(90);
		sf::FloatRect bounds = mCards[0]->getBounds();
		customStates.transform.translate(0.f, -(bounds.height - bounds.width) / 2.f);
		mCards[0]->draw(target, customStates);
		if (mCards.size() > 1)
		{
			mCards[mCards.size() - 1]->draw(target, states);
		}
	}
}
