#include "AI.h"

#include <time.h>
#include <random>
#include <algorithm>

AI::AI() : mTrump(Card::Diamonds)
{
}

void AI::setTrump(Card::Suit trump)
{
	mTrump = trump;
}

AI::Action AI::getCard(CardField::Type type, Card *&result, std::vector<Card *> cards, std::vector<Card *> fieldAttackCards, std::vector<Card *> fieldDefenseCards)
{
	srand(time(NULL));
	result = nullptr;
	std::vector<Card *> availableCards;
	if (type == CardField::Attack)
	{
		if (fieldAttackCards.size() == 0)
		{
			result = getRandom(cards);
			return PutCard;
		}
		else if (fieldAttackCards.size() < 6)
		{
			for (auto card : cards)
			{
				bool isFound = false;
				for (auto attackCard : fieldAttackCards)
				{
					if (card->getRank() == attackCard->getRank())
					{
						availableCards.push_back(card);
						isFound = true;
						break;
					}
				}
				if (isFound)
				{
					continue;
				}
				for (auto defenseCard : fieldDefenseCards)
				{
					if (card->getRank() == defenseCard->getRank())
					{
						availableCards.push_back(card);
						break;
					}
				}
			}
			if (rand() % (availableCards.size() * 3 + 1) == 0)
			{
				return Pass;
			}
			result = getRandom(availableCards);
			return PutCard;
		}
		else
		{
			return Pass;
		}
	}
	else if (type == CardField::Defense)
	{
		if (fieldAttackCards.size() > fieldDefenseCards.size())
		{
			Card *currentCard = fieldAttackCards[fieldDefenseCards.size()];

			for (auto card : cards)
			{
				if (card->getSuit() == mTrump && currentCard->getSuit() != mTrump)
				{
					availableCards.push_back(card);
				}
				else if (card->getSuit() == currentCard->getSuit() &&
						 *card > *currentCard)
				{
					availableCards.push_back(card);
				}
			}
			if (rand() % (availableCards.size() * 4 + 1) == 0)
			{
				return TakeCards;
			}
			result = getRandom(availableCards);
			return PutCard;
		}
		else
		{
			if (rand() % 20 == 0)
			{
				return TakeCards;
			}
			return DiscardCards;
		}
	}
}

bool cardAIComparator(Card *first, Card *second)
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

// need guarantees that the cards is not empty
Card *AI::getRandom(std::vector<Card *> cards)
{
	std::vector<Card *> trumpCards;
	std::vector<Card *> commonCards;
	for (auto card = cards.begin(); card != cards.end(); card++)
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

	std::sort(commonCards.begin(), commonCards.end(), cardAIComparator);
	std::sort(trumpCards.begin(), trumpCards.end(), cardAIComparator);

	cards.clear();
	cards.insert(cards.end(), commonCards.begin(), commonCards.end());
	cards.insert(cards.end(), trumpCards.begin(), trumpCards.end());
	commonCards.clear();
	trumpCards.clear();

	// Change this shit
	// 1, 7, 13, 19, .... where 1 to highest card
	// the biggest part to smallest card

	int temp = 1 + rand() % ((3 * cards.size() - 2) * cards.size());
	int index = 0;

	while (temp > 0)
	{
		temp -= (6 * index + 1);
		index++;
	};

	return cards[cards.size() - index];
}