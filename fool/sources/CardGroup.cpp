#include "CardGroup.h"

#include <stdexcept>
#include <iterator>
#include <algorithm>

CardGroup::CardGroup(Card::Suit trump, PlayerType type)
	: mTrump(trump),
	  mSelectedChild(-1),
	  mCards(),
	  mAreaSize(),
	  mType(type)
{
}

void CardGroup::handleEvent(const sf::Event &event)
{
	if (mType == Player && event.type == sf::Event::MouseMoved)
	{
		bool isNoSelection = true;
		for (int i = mCards.size() - 1; i >= 0; i--)
		{
			mCards[i]->handleEvent(event);
			if (mCards[i]->isSelected())
			{
				if (mSelectedChild != -1 && mSelectedChild != i)
				{
					mCards[mSelectedChild]->deselect();
				}
				isNoSelection = false;
				mSelectedChild = i;
				break;
			}
		}
	}
}

void CardGroup::checkSelection(sf::Vector2f mousePosition)
{
	mSelectedChild = -1;
	for (int i = mCards.size() - 1; i >= 0; i--)
	{
		mCards[i]->checkSelection(mousePosition);
		if (mCards[i]->isSelected())
		{
			mSelectedChild = i;
			break;
		}
	}
}

void CardGroup::pushCard(Card::Ptr card)
{
	mCards.push_back(card.get());
	card->setPosition(0.f, 0.f);
	if (mType == Player)
	{
		card->changeSide(true);
	}
	else
	{
		card->changeSide(false);
	}
	attachChild(std::move(card));
	sort();
}

Card::Ptr CardGroup::getSelectedCard()
{
	if (!hasSelection())
	{
		throw std::runtime_error("CardGroup::getSelectedCard there is no selected card");
	}

	Card::Ptr result(static_cast<Card *>(detachChild((*mCards[mSelectedChild])).release()));
	result->move(getPosition());
	auto it = mCards.begin();
	std::advance(it, mSelectedChild);
	mCards.erase(it);
	sort();
	return std::move(result);
}

Card::Ptr CardGroup::getCard(const Card &card)
{
	Card::Ptr result(static_cast<Card *>(detachChild(card).release()));
	result->move(getPosition());
	auto it = std::find(mCards.begin(), mCards.end(), &card);
	mCards.erase(it);
	sort();
	return std::move(result);
}

bool CardGroup::hasSelection() const
{
	return mSelectedChild >= 0;
}

bool CardGroup::getMinimumTrump(Card::Rank &minRank) const
{
	minRank = Card::_Ace;
	bool result = false;
	for (auto card = mCards.begin(); card != mCards.end(); card++)
	{
		if ((*card)->getSuit() == mTrump)
		{
			result = true;
			if ((*card)->getRank() < minRank)
			{
				minRank = (*card)->getRank();
			}
		}
	}

	return result;
}

int CardGroup::getNumberOfCards() const
{
	return mCards.size();
}

std::vector<Card *> CardGroup::getCards()
{
	return mCards;
}

bool cardComparator(Card *first, Card *second)
{
	if (first->getRank() != second->getRank())
	{
		return first->getRank() < second->getRank();
	}
	else
	{
		return first->getSuit() < second->getSuit();
	}
}

void CardGroup::sort()
{
	if (mCards.size() > 0)
	{
		if (mCards.size() == 1)
		{
			mCards[0]->setPosition(0.f, 0.f);
			return;
		}

		if (mType == Player)
		{
			std::vector<Card *> trumpCards;
			std::vector<Card *> commonCards;
			for (auto card = mCards.begin(); card != mCards.end(); card++)
			{
				if ((*card)->getSuit() == mTrump)
				{
					trumpCards.push_back(*card);
				}
				else
				{
					commonCards.push_back(*card);
				}
			}

			std::sort(commonCards.begin(), commonCards.end(), cardComparator);
			std::sort(trumpCards.begin(), trumpCards.end(), cardComparator);

			mCards.clear();
			mCards.insert(mCards.end(), commonCards.begin(), commonCards.end());
			mCards.insert(mCards.end(), trumpCards.begin(), trumpCards.end());
			commonCards.clear();
			trumpCards.clear();
		}

		sf::FloatRect cardBounds = mCards[0]->getGlobalBounds();
		sf::Vector2f areaSize = getWorldSize();

		if ((mCards.size() * cardBounds.width * 0.9f + 0.1f * cardBounds.width) < areaSize.x)
		{
			areaSize = sf::Vector2f(mCards.size() * cardBounds.width * 0.9f + 0.1f * cardBounds.width, areaSize.y);
		}
		float position = -areaSize.x / 2.f + cardBounds.width / 2.f;
		float step = (areaSize.x - cardBounds.width) / (mCards.size() - 1);
		for (int i = 0; i < mCards.size(); i++, position += step)
		{
			mCards[i]->setPosition(position, 0.f);
		}
	}
}

void CardGroup::setLocalSize(sf::Vector2f size)
{
	mAreaSize = size;
}

sf::Vector2f CardGroup::getWorldSize()
{
	sf::FloatRect localBound(-mAreaSize / 2.f, mAreaSize);
	localBound = getTransform().transformRect(localBound);
	return sf::Vector2f(localBound.width, localBound.height);
}

void CardGroup::drawChildren(sf::RenderTarget &target, sf::RenderStates states) const
{
	for (auto card = mCards.begin(); card != mCards.end(); card++)
	{
		(*card)->draw(target, states);
	}
}