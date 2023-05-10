#pragma once

#include "Engine/SceneNode.h"
#include "Game/Card.h"

#include <vector>

class CardPile : public SceneNode
{
public:
	typedef std::unique_ptr<CardPile> Ptr;

public:
	CardPile();

	virtual void handleEvent(const sf::Event &event);

	void pushCard(Card::Ptr card);

private:
	bool isEmpty() const;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	std::vector<Card *> mCards;
};
