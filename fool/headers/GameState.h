#pragma once

#include "AI.h"
#include "State.h"
#include "GameStatus.h"
#include "Container.h"
#include "Button.h"
#include "Label.h"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.h"
#include "PlayerIdentifiers.h"
#include "Card.h"
#include "CardDeck.h"
#include "CardGroup.h"
#include "CardPile.h"
#include "CardField.h"

#include <vector>

class GameState : public State
{
private:
	enum Status
	{
		DealCards,
		PlayerTurn,
		EnemyTurn,
		ClearField
	};

public:
	GameState(StateStack &stack, Context context);
	~GameState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event &event);

private:
	TextureHolder mTextures;

	AI mAI;

	sf::Time elapsedTime;

	SceneNode mSceneGraph;
	CardDeck *mCardDeck;
	CardGroup *mPlayerCards;
	CardGroup *mEnemyCards;
	CardPile *mCardPile;
	CardField *mCardField;
	GUI::Container *mGUIContainer;
	GUI::Button *mLeftButton;
	GUI::Button *mRightButton;
	GUI::Label *mMainLabel;

	AI::Action mPlayerAction;
	PlayerType mCurrentDefender;
	Status mCurrentStatus;
	Card::Suit mTrump;

	bool mAvailableSelection;
};