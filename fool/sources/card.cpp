#include "Card.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <stdexcept>

Card::Card(Suit suit, Rank rank, const TextureHolder &textures) : suit(suit), rank(rank)
{
	sprite.setTexture(textures.get(Textures::ID((Textures::Global::ID)suit, (Textures::Local::ID)rank)));
}

Card::Rank Card::getRank()
{
	return this->rank;
}

Card::Suit Card::getSuit()
{
	return this->suit;
}

void Card::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(sprite, states);
}