#include "Game/CardDeck.h"
#include "Engine/SpriteNode.h"

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
	card->setPosition(0.f, 0.f);
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
	result->setPosition(getPosition());
	return std::move(result);
}

Card::Suit CardDeck::getTrump()
{
	return mTrump;
}

bool CardDeck::isEmpty() const
{
	return mCards.empty();
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
	mTrump = mCards[0]->getSuit();
}

void CardDeck::applyShuffle()
{
	for (Card *child : mCards)
	{
		child->changeSide(false);
	}
	mCards[0]->changeSide(true);
	mTrump = mCards[0]->getSuit();
}

void CardDeck::getDeckDescription(int deck[][2])
{
	for (size_t i = 0; i < mCards.size(); i++)
	{
		deck[i][0] = (int)mCards[i]->getSuit();
		deck[i][1] = (int)mCards[i]->getRank();
	}
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
		sf::FloatRect bounds = mCards[0]->getGlobalBounds();
		customStates.transform.translate(0.f, -(bounds.height - bounds.width) / 2.f);
		mCards[0]->draw(target, customStates);
		if (mCards.size() > 1)
		{
			mCards[mCards.size() - 1]->draw(target, states);
		}
	}
}
