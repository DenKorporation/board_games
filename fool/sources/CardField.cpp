#include "CardField.h"
#include "Animation.h"

#include "stdexcept"
CardField::CardField(Card::Suit trump)
	: mAttackCards(),
	  mDefenseCards(),
	  mAreaSize(),
	  mTrump(trump)
{
}

void CardField::handleEvent(const sf::Event &event)
{
}

void CardField::setTrump(Card::Suit trump)
{
	mTrump = trump;
}

void CardField::pushCard(Card::Ptr card, Type type)
{
	if (type == Attack)
	{
		mAttackCards.push_back(card.get());
	}
	else if (type == Defense)
	{
		mDefenseCards.push_back(card.get());
	}

	card->setPosition(0.f, 0.f);
	card->changeSide(true);
	attachChild(std::move(card));
	setCardPlace();
}

bool CardField::checkCard(Card *card, Type type) const
{
	if (type == Attack && mAttackCards.size() == 0)
	{
		return true;
	}
	if (type == Attack && mAttackCards.size() < 6)
	{
		Card::Rank rank = card->getRank();
		for (auto itr = mAttackCards.begin(); itr != mAttackCards.end(); itr++)
		{
			if ((*itr)->getRank() == rank)
			{
				return true;
			}
		}
		for (auto itr = mDefenseCards.begin(); itr != mDefenseCards.end(); itr++)
		{
			if ((*itr)->getRank() == rank)
			{
				return true;
			}
		}
	}
	else if (type == Defense && mDefenseCards.size() < mAttackCards.size())
	{
		int current = mDefenseCards.size();
		if (card->getSuit() == mTrump && mAttackCards[current]->getSuit() != mTrump)
		{
			return true;
		}
		else if (card->getSuit() == mAttackCards[current]->getSuit() &&
				 *card > *mAttackCards[current])
		{
			return true;
		}
	}

	return false;
}

void CardField::setLocalSize(sf::Vector2f size)
{
	mAreaSize = size;
}

int CardField::getNumberOfAttackCards() const
{
	return mAttackCards.size();
}

int CardField::getNumberOfDefenseCards() const
{
	return mDefenseCards.size();
}

sf::Vector2f CardField::getPlace(Type type, Card *card)
{
	sf::FloatRect cardBounds = card->getGlobalBounds();
	sf::Vector2f areaSize = getWorldSize();

	sf::Vector2f position = sf::Vector2f(-areaSize.x * 2.5f / 6, 0.f);
	float step = areaSize.x / 6.f;
	if (type == CardField::Attack)
	{
		position.x += step * getNumberOfAttackCards();
		position.x += -cardBounds.width * 0.05f;
		position.y += -cardBounds.height * 0.05f;
	}
	else
	{
		position.x += step * getNumberOfDefenseCards();
		position.x += cardBounds.width * 0.05f;
		position.y += cardBounds.height * 0.05f;
	}
	position += getPosition();
	return position;
}

std::vector<Card *> CardField::getAttackCards()
{
	return mAttackCards;
}

std::vector<Card *> CardField::getDefenseCards()
{
	return mDefenseCards;
}

std::vector<Animation *> CardField::clearFields(SceneNode *destination, const SoundHolder &sounds)
{
	sf::Time animationTime = sf::seconds(1.f);
	// if (dynamic_cast<CardGroup *>(destination) != nullptr)
	// {
	// 	animationTime = sf::seconds(0.5f);
	// }

	std::vector<Animation *> animations;
	int count = 0;
	for (auto card = mAttackCards.begin(); card != mAttackCards.end(); card++)
	{
		(*card)->move(getPosition());
		animations.push_back(new Animation(*card, mParent, static_cast<SceneNode *>(destination), (*card)->getPosition(), destination->getPosition(), animationTime, sounds));
		animations[animations.size() - 1]->setDelayTime(sf::seconds(0.1f * count));
		count++;
		mParent->attachChild(detachChild(**card));
	}
	mAttackCards.clear();
	count = 0;
	for (auto card = mDefenseCards.begin(); card != mDefenseCards.end(); card++)
	{
		(*card)->move(getPosition());
		animations.push_back(new Animation(*card, mParent, static_cast<SceneNode *>(destination), (*card)->getPosition(), destination->getPosition(), animationTime, sounds));
		animations[animations.size() - 1]->setDelayTime(sf::seconds(0.05f + 0.1f * count));
		mParent->attachChild(detachChild(**card));
		count++;
	}
	mDefenseCards.clear();
	return animations;
}

sf::Vector2f CardField::getWorldSize()
{
	sf::FloatRect localBound(-mAreaSize / 2.f, mAreaSize);
	localBound = getTransform().transformRect(localBound);
	return sf::Vector2f(localBound.width, localBound.height);
}

void CardField::setCardPlace()
{
	if (mAttackCards.size() > 0)
	{
		sf::FloatRect cardBounds = mAttackCards[0]->getGlobalBounds();
		sf::Vector2f areaSize = getWorldSize();

		sf::Vector2f position = sf::Vector2f(-areaSize.x * 2.5f / 6, 0.f);
		float step = areaSize.x / 6.f;
		for (int i = 0; i < mAttackCards.size(); i++)
		{
			mAttackCards[i]->setPosition(position);
			mAttackCards[i]->move(-cardBounds.width * 0.05f, -cardBounds.height * 0.05f);
			if (i < mDefenseCards.size())
			{
				mDefenseCards[i]->setPosition(position);
				mDefenseCards[i]->move(cardBounds.width * 0.05f, cardBounds.height * 0.05f);
			}
			position.x += step;
		}
	}
}

void CardField::drawChildren(sf::RenderTarget &target, sf::RenderStates states) const
{
	for (auto card = mAttackCards.begin(); card != mAttackCards.end(); card++)
	{
		(*card)->draw(target, states);
	}
	for (auto card = mDefenseCards.begin(); card != mDefenseCards.end(); card++)
	{
		(*card)->draw(target, states);
	}
}
