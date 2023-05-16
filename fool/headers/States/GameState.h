#pragma once

#include "Engine/Resource/ResourceHolder.hpp"
#include "Engine/Resource/ResourceIdentifiers.h"
#include "Engine/State.h"
#include "GUI/Container.h"
#include "GUI/Button.h"
#include "GUI/Label.h"
#include "Game/Card.h"
#include "Game/CardDeck.h"
#include "Game/CardGroup.h"
#include "Game/CardPile.h"
#include "Game/CardField.h"
#include "Game/Animation.h"
#include "Game/AI.h"
#include "Game/GameInfo.h"
#include "Game/PlayerIdentifiers.h"

#include <SFML/Audio/Sound.hpp>

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

	std::vector<Animation *> mAnimations;

	sf::Time elapsedTime;
	sf::Time mLabelElapsedTime;

	sf::Sound mErrorSound;
	sf::Sound mDealCardsSound;

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
	bool mTakesConfirmation;
};