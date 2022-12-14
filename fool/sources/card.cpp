#include "Card.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <stdexcept>

Card::Card(Suit suit, Rank rank, const TextureHolder &textures) : mSuit(suit), mRank(rank)
{
	mFrontFaceSprite.setTexture(textures.get(Textures::ID((Textures::Global::ID)suit, (Textures::Local::ID)rank)));
	mReverseFaceSprite.setTexture(textures.get(Textures::ID((Textures::Global::ReverseFace))));
}

Card::Rank Card::getRank()
{
	return this->mRank;
}

Card::Suit Card::getSuit()
{
	return this->mSuit;
}

void Card::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mFrontFaceSprite, states);
}