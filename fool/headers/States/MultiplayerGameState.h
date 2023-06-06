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
#include "ServerService.h"

#include <SFML/Audio/Sound.hpp>

#include <vector>
#include <thread>
#include <future>
#include <mutex>
#include <queue>

class MultiplayerGameState : public State
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
	MultiplayerGameState(StateStack &stack, Context context);
	~MultiplayerGameState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event &event);

private:
	void BackgroundAction(std::future<void> futureObj);
	bool HandleAction(AI::Action &action, Card *&result, std::vector<Card *> cards);

private:
	TextureHolder mTextures;

	std::queue<json> mPendingActions;
	std::mutex mPendingMutex;
	std::mutex mCommonMutex;

	std::unique_ptr<ServerService> mService;
	std::thread mBackgroundThread;
	std::promise<void> mPromise;
	std::future<void> mFutureObj;

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