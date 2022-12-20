#pragma once

#include "State.h"
#include "GameStatus.h"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.h"
#include "Card.h"
#include "CardDeck.h"
#include "CardGroup.h"

#include <vector>

class GameState : public State
{
public:
	GameState(StateStack &stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event &event);

private:
	TextureHolder mTextures;
	Card::Suit mTrump;

	SceneNode mSceneGraph;
	CardDeck *mCardDeck;
	CardGroup *mPlayerCards;
	CardGroup *mEnemyCards;
};