#include "GameState.h"
#include "Utility.h"
#include "SpriteNode.h"

#include <SFML/Graphics/RenderWindow.hpp>

GameState::GameState(StateStack &stack, Context context)
	: State(stack, context),
	  mTextures(),
	  mSceneGraph()
{
	for (Card::Suit suit = Card::Diamonds; suit <= Card::Spades; suit = Card::Suit((int)suit + 1))
	{
		for (Card::Rank rank = Card::_6; rank <= Card::_Ace; rank = Card::Rank((int)rank + 1))
		{
			mTextures.load(Textures::ID((Textures::Global::ID)suit, (Textures::Local::ID)rank), getFilePath(suit, rank));
		}
		mTextures.load(Textures::ID((Textures::Global::ID)suit, Textures::Local::Main), getFilePath((Textures::Global::ID)suit, Textures::Local::Main));
	}
	mTextures.load(Textures::ID(Textures::Global::ReverseFace), getFilePath(Textures::Global::ReverseFace));

	sf::Vector2f currentWindowSize = sf::Vector2f(context.window->getSize().x, context.window->getSize().y);
	sf::Vector2f defaultWindowSize = sf::Vector2f(1920.f, 1080.f);
	sf::Vector2f windowScale = sf::Vector2f(currentWindowSize.x / defaultWindowSize.x, currentWindowSize.y / defaultWindowSize.y);
	sf::Texture &cardTexture = mTextures.get(Textures::ID(Textures::Global::ReverseFace));
	sf::Vector2f cardCurrentSize = sf::Vector2f(cardTexture.getSize().x * windowScale.x, cardTexture.getSize().y * windowScale.y);

	sf::Texture &texture = context.textures->get(Textures::ID(Textures::Global::Background));
	SpriteNode::Ptr backgroundSprite(new SpriteNode(texture));
	backgroundSprite->setScale(currentWindowSize.x / texture.getSize().x, currentWindowSize.y / texture.getSize().y);
	backgroundSprite->setPosition(currentWindowSize / 2.f);
	mSceneGraph.attachChild(std::move(backgroundSprite));

	CardDeck::Ptr cardDeck(new CardDeck());
	cardDeck->setPosition(cardCurrentSize * 0.7f);
	mCardDeck = cardDeck.get();
	mSceneGraph.attachChild(std::move(cardDeck));

	for (Card::Suit suit = Card::Diamonds; suit <= Card::Spades; suit = Card::Suit((int)suit + 1))
	{
		for (Card::Rank rank = Card::_6; rank <= Card::_Ace; rank = Card::Rank((int)rank + 1))
		{
			Card::Ptr card(new Card(suit, rank, mTextures));
			card->setScale(windowScale);
			mCardDeck->pushCard(std::move(card));
		}
	}
	mCardDeck->shuffle();
	SpriteNode::Ptr suitSprite(new SpriteNode(mTextures.get(Textures::ID((Textures::Global::ID)mCardDeck->getTrump(), Textures::Local::Main))));
	suitSprite->setScale(windowScale);
	mCardDeck->attachChild(std::move(suitSprite));

	mAI.setTrump(mCardDeck->getTrump());

	CardGroup::Ptr playerCards(new CardGroup(mCardDeck->getTrump(), Player));
	mPlayerCards = playerCards.get();
	mSceneGraph.attachChild(std::move(playerCards));

	mPlayerCards->setPosition(currentWindowSize.x / 2.f, currentWindowSize.y - cardCurrentSize.y * 0.6f);
	mPlayerCards->setLocalSize(sf::Vector2f(currentWindowSize.x / 2.f, cardCurrentSize.y));

	CardGroup::Ptr enemyCards(new CardGroup(mCardDeck->getTrump(), Enemy));
	mEnemyCards = enemyCards.get();
	mSceneGraph.attachChild(std::move(enemyCards));

	mEnemyCards->setPosition(currentWindowSize.x / 2.f, cardCurrentSize.y * 0.6f);
	mEnemyCards->setLocalSize(sf::Vector2f(currentWindowSize.x / 2.f, cardCurrentSize.y));

	// Deal of cards
	for (size_t i = 0; i < 6; i++)
	{
		mPlayerCards->pushCard(mCardDeck->popCard());
		mEnemyCards->pushCard(mCardDeck->popCard());
	}

	Card::Rank playerMinTrump, enemyMinTrump;

	if (mEnemyCards->getMinimumTrump(enemyMinTrump))
	{
		if (mPlayerCards->getMinimumTrump(playerMinTrump) && (playerMinTrump < enemyMinTrump))
		{
			mCurrentTurn = Player;
			mCurrentDefender = Enemy;
			mCurrentStatus = PlayerTurn;
		}
		else
		{
			mCurrentTurn = Enemy;
			mCurrentDefender = Player;
			mCurrentStatus = EnemyTurn;
		}
	}
	else
	{
		mCurrentTurn = Player;
		mCurrentDefender = Enemy;
		mCurrentStatus = PlayerTurn;
	}

	CardPile::Ptr cardPile(new CardPile());
	mCardPile = cardPile.get();
	cardPile->setPosition(currentWindowSize.x - cardCurrentSize.x * 0.7f, cardCurrentSize.y * 0.7f);
	mSceneGraph.attachChild(std::move(cardPile));

	CardField::Ptr cardField(new CardField(mTrump));
	mCardField = cardField.get();
	cardField->setPosition(currentWindowSize / 2.f);
	cardField->setLocalSize(sf::Vector2f(currentWindowSize.x * 2.f / 3.f, currentWindowSize.y / 2.f));
	mSceneGraph.attachChild(std::move(cardField));

	context.gameStatus->setCurrentStatus(GameStatus::InGame);
}

GameState::~GameState()
{
	auto statistics = getStatistics();
	statistics["total"]++;
	switch (getContext().gameStatus->getCurrentStatus())
	{
	case GameStatus::PlayerWon:
		statistics["victory"]++;
		break;
	case GameStatus::ComputerWon:
		statistics["lose"]++;
		break;
	case GameStatus::Draw:
		statistics["draw"]++;
		break;
	}
	setStatistics(statistics);
}

void GameState::draw()
{
	sf::RenderWindow &window = *getContext().window;
	window.draw(mSceneGraph);
}

bool GameState::update(sf::Time dt)
{
	return true;
}

bool GameState::handleEvent(const sf::Event &event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			requestStackPush(States::Pause);
			break;
		}
	}
	if (mCurrentStatus == PlayerTurn)
	{
		mPlayerCards->handleEvent(event);
	}
	return true;
}