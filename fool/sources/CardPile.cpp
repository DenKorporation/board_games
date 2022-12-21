#include "CardPile.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

CardPile::CardPile()
{
}

void CardPile::handleEvent(const sf::Event &event)
{
}

void CardPile::pushCard(Card::Ptr card)
{
	card->setPosition(0.f, 0.f);
	card->changeSide(false);
	mCards.push_back(card.get());
	attachChild(std::move(card));
}

bool CardPile::isEmpty() const
{
	return mCards.size() == 0;
}

void CardPile::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	if (!isEmpty())
	{
		target.draw(*mCards[0], states);
	}
}
