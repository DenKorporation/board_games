#pragma once

#include "Game/Card.h"
#include "Game/CardField.h"

#include <vector>

class AI
{
public:
	enum Action
	{
		None,
		Pass,
		DiscardCards,
		PutCard,
		TakeCards
	};

public:
	AI();

	void setTrump(Card::Suit trump);

	// return nullptr or pointer to Card
	Action getCard(CardField::Type type, Card *&result, std::vector<Card *> cards, std::vector<Card *> fieldAttackCards, std::vector<Card *> fieldDefenseCards);

private:
	Card *getRandom(std::vector<Card *> cards);

private:
	Card::Suit mTrump;
};