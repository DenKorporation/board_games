#pragma once

#include "SceneNode.h"
#include "Card.h"

#include <vector>

class CardGroup : public SceneNode
{
public:
	typedef std::unique_ptr<CardGroup> Ptr;
	enum Type
	{
		Player,
		Enemy
	};

public:
	CardGroup(Card::Suit trump, Type type);

	virtual void handleEvent(const sf::Event &event);
	void checkSelection(sf::Vector2f mousePosition);
	void pushCard(Card::Ptr card);
	Card::Ptr getSelectedCard();
	void setLocalSize(sf::Vector2f size);
	bool hasSelection() const;

private:
	void sort();
	sf::Vector2f getWorldSize();
	virtual void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	std::vector<Card *> mCards;
	sf::Vector2f mAreaSize;
	Card::Suit mTrump;
	unsigned int mSelectedChild;
	Type mType;
};
