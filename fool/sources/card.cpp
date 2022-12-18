#include "Card.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <stdexcept>

Card::Card(Suit suit, Rank rank, const TextureHolder &textures) : mSuit(suit), mRank(rank), misFaceSide(true), mIsSelected(false)
{
	mFrontFaceSprite.setTexture(textures.get(Textures::ID((Textures::Global::ID)suit, (Textures::Local::ID)rank)));
	mReverseFaceSprite.setTexture(textures.get(Textures::ID((Textures::Global::ReverseFace))));
	centerOrigin(mFrontFaceSprite);
	centerOrigin(mReverseFaceSprite);
}

Card::Rank Card::getRank()
{
	return this->mRank;
}

Card::Suit Card::getSuit()
{
	return this->mSuit;
}

void Card::changeSide(bool isFaceSide)
{
	misFaceSide = isFaceSide;
}

void Card::handleEvent(const sf::Event &event)
{
}

sf::FloatRect Card::getBounds() const
{
	return getWorldTransform().transformRect(mFrontFaceSprite.getGlobalBounds());
}

void Card::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (misFaceSide)
	{
		target.draw(mFrontFaceSprite, states);
	}
	else
	{
		target.draw(mReverseFaceSprite, states);
	}
}