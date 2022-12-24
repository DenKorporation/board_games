#pragma once

#include "SceneNode.h"
#include "Card.h"
#include "CardGroup.h"
#include "CardPile.h"

#include <vector>

class Animation;

class CardField : public SceneNode
{
public:
	typedef std::unique_ptr<CardField> Ptr;
	enum Type
	{
		Attack,
		Defense
	};

public:
	CardField(Card::Suit trump);

	virtual void handleEvent(const sf::Event &event);

	void setTrump(Card::Suit trump);
	void pushCard(Card::Ptr card, Type type);
	bool checkCard(Card *card, Type type) const;
	void setLocalSize(sf::Vector2f size);
	std::vector<Card *> getAttackCards();
	std::vector<Card *> getDefenseCards();
	int getNumberOfAttackCards() const;
	int getNumberOfDefenseCards() const;
	std::vector<Animation *> clearFields(SceneNode *destination);
	sf::Vector2f getPlace(Type type, Card *card);

private:
	sf::Vector2f getWorldSize();
	void setCardPlace();
	virtual void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	std::vector<Card *> mAttackCards;
	std::vector<Card *> mDefenseCards;
	sf::Vector2f mAreaSize;
	Card::Suit mTrump;
};
