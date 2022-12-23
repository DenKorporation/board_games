#pragma once

#include "SceneNode.h"
#include "PlayerIdentifiers.h"
#include "Card.h"

#include <vector>

class CardGroup : public SceneNode
{
public:
	typedef std::unique_ptr<CardGroup> Ptr;

public:
	CardGroup(Card::Suit trump, PlayerType type);

	virtual void handleEvent(const sf::Event &event);
	void checkSelection(sf::Vector2f mousePosition);
	void pushCard(Card::Ptr card);
	Card *getSelectedCard();
	Card::Ptr getCard(const Card &card);
	void setLocalSize(sf::Vector2f size);
	bool hasSelection() const;
	bool getMinimumTrump(Card::Rank &minRank) const;
	std::vector<Card *> getCards();
	int getNumberOfCards() const;

private:
	void sort();
	sf::Vector2f getWorldSize();
	virtual void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	std::vector<Card *> mCards;
	sf::Vector2f mAreaSize;
	Card::Suit mTrump;
	int mSelectedChild;
	PlayerType mType;
};
