#include "Game/Card.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <stdexcept>

Card::Card(Suit suit, Rank rank, const TextureHolder &textures, SoundHolder &sounds) : mSuit(suit), mRank(rank), misFaceSide(true), mIsSelected(false)
{
	mSelectedSound.setBuffer(sounds.get(Sounds::CardSelect));

	mFrontFaceSprite.setTexture(textures.get(Textures::ID((Textures::Global::ID)suit, (Textures::Local::ID)rank)));
	mReverseFaceSprite.setTexture(textures.get(Textures::ID((Textures::Global::ReverseFace))));
	centerOrigin(mFrontFaceSprite);
	centerOrigin(mReverseFaceSprite);
}

Card::Rank Card::getRank() const
{
	return this->mRank;
}

Card::Suit Card::getSuit() const
{
	return this->mSuit;
}

void Card::changeSide(bool isFaceSide)
{
	misFaceSide = isFaceSide;
}

void Card::handleEvent(const sf::Event &event)
{
	if (event.type == sf::Event::MouseMoved)
	{
		checkSelection(sf::Vector2f((float)event.mouseMove.x, (float)event.mouseMove.y));
	}
}

void Card::checkSelection(sf::Vector2f mousePosition)
{
	sf::FloatRect bounds = getWorldTransform().transformRect(mFrontFaceSprite.getGlobalBounds());
	float checkHeight = bounds.height;
	if (isSelected())
	{
		checkHeight *= 1.3f;
	}

	if ((mousePosition.x >= bounds.left) && (mousePosition.x <= bounds.left + bounds.width) &&
		(mousePosition.y >= bounds.top) && (mousePosition.y <= bounds.top + checkHeight))
	{
		if (!isSelected())
		{
			select();
		}
	}
	else if (isSelected())
	{
		deselect();
	}
}

sf::FloatRect Card::getGlobalBounds() const
{
	return getWorldTransform().transformRect(mFrontFaceSprite.getGlobalBounds());
}

bool Card::isSelected() const
{
	return mIsSelected;
}

void Card::select()
{
	mIsSelected = true;
	mFrontFaceSprite.setPosition(0.f, -0.3f * getGlobalBounds().height);
	mReverseFaceSprite.setPosition(0.f, -0.3f * getGlobalBounds().height);
	mSelectedSound.play();
}

void Card::deselect()
{
	mIsSelected = false;
	mFrontFaceSprite.setPosition(0.f, 0.f);
	mReverseFaceSprite.setPosition(0.f, 0.f);
}

bool Card::operator<(const Card &compareTo)
{
	return this->mRank < compareTo.mRank;
}

bool Card::operator>(const Card &compareTo)
{
	return this->mRank > compareTo.mRank;
}

bool Card::operator==(const Card &compareTo)
{
	return this->mRank == compareTo.mRank;
}

bool Card::operator<=(const Card &compareTo)
{
	return this->mRank <= compareTo.mRank;
}

bool Card::operator>=(const Card &compareTo)
{
	return this->mRank >= compareTo.mRank;
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