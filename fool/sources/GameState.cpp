#include "GameState.h"
#include "Utility.h"
#include "SpriteNode.h"
#include "GameStatus.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <stdexcept>

GameState::GameState(StateStack &stack, Context context)
	: State(stack, context),
	  mTextures(),
	  mSceneGraph(),
	  mPlayerAction(AI::None),
	  mAvailableSelection(true),
	  mTakesConfirmation(false)
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

	CardPile::Ptr cardPile(new CardPile());
	mCardPile = cardPile.get();
	cardPile->setPosition(currentWindowSize.x - cardCurrentSize.x * 0.7f, cardCurrentSize.y * 0.7f);
	mSceneGraph.attachChild(std::move(cardPile));

	CardField::Ptr cardField(new CardField(mTrump));
	mCardField = cardField.get();
	cardField->setPosition(currentWindowSize / 2.f);
	cardField->setLocalSize(sf::Vector2f(currentWindowSize.x * 2.f / 3.f, currentWindowSize.y / 2.f));
	cardField->setTrump(mCardDeck->getTrump());
	mSceneGraph.attachChild(std::move(cardField));

	GUI::Container::Ptr GUIContainer(new GUI::Container());
	mGUIContainer = GUIContainer.get();
	mSceneGraph.attachChild(std::move(GUIContainer));
	mGUIContainer->setPosition(currentWindowSize.x / 2.f, 0.f);

	GUI::Button::Ptr leftBtn(new GUI::Button(*context.fonts));
	mLeftButton = leftBtn.get();
	mGUIContainer->attachChild(std::move(leftBtn));
	mLeftButton->setPosition(-currentWindowSize.x / 3.f, currentWindowSize.y - cardCurrentSize.y * 0.6f);
	mLeftButton->setSize(sf::Vector2f(currentWindowSize.x / 8.f, cardCurrentSize.y * 0.8f));
	mLeftButton->setNormalStyle(GUI::Button::Style(cardCurrentSize.y * 0.3f, sf::Color::White, sf::Color(105, 105, 105, 150),
												   sf::Color::Black, 3.f));
	mLeftButton->setSelectedStyle(GUI::Button::Style(cardCurrentSize.y * 0.3f, sf::Color::Red, sf::Color(105, 105, 105, 150),
													 sf::Color::Black, 3.f));

	GUI::Button::Ptr rightBtn(new GUI::Button(*context.fonts));
	mRightButton = rightBtn.get();
	mGUIContainer->attachChild(std::move(rightBtn));
	mRightButton->setPosition(currentWindowSize.x / 3.f, currentWindowSize.y - cardCurrentSize.y * 0.6f);
	mRightButton->setSize(sf::Vector2f(currentWindowSize.x / 8.f, cardCurrentSize.y * 0.8f));
	mRightButton->setNormalStyle(GUI::Button::Style(cardCurrentSize.y * 0.25f, sf::Color::White, sf::Color(105, 105, 105, 150),
													sf::Color::Black, 3.f));
	mRightButton->setSelectedStyle(GUI::Button::Style(cardCurrentSize.y * 0.25f, sf::Color::Red, sf::Color(105, 105, 105, 150),
													  sf::Color::Black, 3.f));

	GUI::Label::Ptr mainLabel(new GUI::Label("", *context.fonts, Fonts::Main));
	mMainLabel = mainLabel.get();
	mainLabel->setFontSize((unsigned int)currentWindowSize.y / 12);
	mainLabel->setPosition(0.f, currentWindowSize.y / 2.f);
	mainLabel->setOutlineThickness(3.f);
	// mainLabel->setFillColor(sf::Color(200, 200, 200));
	mGUIContainer->attachChild(std::move(mainLabel));

	mCurrentStatus = DealCards;
	context.gameStatus->setCurrentStatus(GameStatus::InGame);

	// Deal of cards
	for (size_t i = 0; i < 6; i++)
	{
		Card::Ptr playerCard = mCardDeck->popCard();
		mAnimations.push_back(new Animation(playerCard.get(), &mSceneGraph, static_cast<SceneNode *>(mPlayerCards),
											playerCard->getPosition(), mPlayerCards->getPosition(), sf::seconds(1.f)));
		mAnimations[mAnimations.size() - 1]->setDelayTime(sf::seconds(i * 0.2f));
		mSceneGraph.attachChild(std::move(playerCard));
		Card::Ptr enemyCard = mCardDeck->popCard();
		mAnimations.push_back(new Animation(enemyCard.get(), &mSceneGraph, static_cast<SceneNode *>(mEnemyCards),
											enemyCard->getPosition(), mEnemyCards->getPosition(), sf::seconds(1.f)));
		mAnimations[mAnimations.size() - 1]->setDelayTime(sf::seconds(0.1f + i * 0.2f));
		mSceneGraph.attachChild(std::move(enemyCard));
	}
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
	case GameStatus::EnemyWon:
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
	mLabelElapsedTime += dt;
	if (mLabelElapsedTime > sf::seconds(2.f))
	{
		mMainLabel->setText("");
		mMainLabel->setFillColor(sf::Color::White);
	}

	if (!mAnimations.empty())
	{
		for (auto it = mAnimations.begin(); it != mAnimations.end();)
		{
			if ((*it)->update(dt))
			{
				(*it)->release();
				it = mAnimations.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
	else
	{

		switch (mCurrentStatus)
		{
		case PlayerTurn:
		{
			Card *result;
			CardField::Type type = (mCurrentDefender == Player) ? CardField::Defense : CardField::Attack;

			if (type == CardField::Attack && mPlayerAction == AI::PutCard)
			{
				if (mCardField->getNumberOfAttackCards() >= 6 ||
					mCardField->getNumberOfAttackCards() - mCardField->getNumberOfDefenseCards() >= mEnemyCards->getNumberOfCards())
				{
					mPlayerAction = AI::None;
					mMainLabel->setText("Incorrect action");
					mMainLabel->setFillColor(sf::Color::Red);
					mLabelElapsedTime = sf::Time::Zero;
				}
			}

			switch (mPlayerAction)
			{
			case AI::PutCard:
			{
				result = mPlayerCards->getSelectedCard();
				if (!mCardField->checkCard(result, type))
				{
					mMainLabel->setText("Incorrect action");
					mMainLabel->setFillColor(sf::Color::Red);
					mLabelElapsedTime = sf::Time::Zero;
					break;
				}
				Card::Ptr card = mPlayerCards->getCard(*result);
				mAnimations.push_back(new Animation(card.get(), &mSceneGraph, static_cast<SceneNode *>(mCardField), type,
													card->getPosition(), mCardField->getPlace(type, card.get()), sf::seconds(0.5f)));
				mSceneGraph.attachChild(std::move(card));

				if (!mTakesConfirmation)
				{
					if (type == CardField::Attack)
					{
						mCurrentStatus = EnemyTurn;
						mRightButton->clearTextAndCallback();
						elapsedTime = sf::Time::Zero;
					}
					else if (mEnemyCards->getNumberOfCards() == 0)
					{
						mLeftButton->setText("Take it");
						mLeftButton->setCallback([this]()
												 { mPlayerAction = AI::TakeCards; });
						mRightButton->setText("Discard");
						mRightButton->setCallback([this]()
												  { mPlayerAction = AI::DiscardCards; });
						mAvailableSelection = false;
					}
					else
					{
						mCurrentStatus = EnemyTurn;
						mLeftButton->clearTextAndCallback();
						elapsedTime = sf::Time::Zero;
					}
				}
				break;
			}
			case AI::TakeCards:
				if (mPlayerCards->getNumberOfCards() == 0)
				{
					std::vector<Animation *> temp = mCardField->clearFields(mPlayerCards);
					mAnimations.insert(mAnimations.end(), temp.begin(), temp.end());
					mCurrentDefender = Player;
					mCurrentStatus = ClearField;
				}
				else
				{
					mTakesConfirmation = true;
					mLeftButton->clearTextAndCallback();
					mRightButton->clearTextAndCallback();
					mCurrentStatus = EnemyTurn;
				}
				elapsedTime = sf::Time::Zero;
				break;
			case AI::Pass:
				if (mTakesConfirmation)
				{
					std::vector<Animation *> temp = mCardField->clearFields(mEnemyCards);
					mAnimations.insert(mAnimations.end(), temp.begin(), temp.end());
					mCurrentDefender = Enemy;
					mCurrentStatus = ClearField;
					mTakesConfirmation = false;
				}
				else
				{
					mCurrentStatus = EnemyTurn;
					mRightButton->clearTextAndCallback();
				}
				elapsedTime = sf::Time::Zero;
				break;
			case AI::DiscardCards:
				std::vector<Animation *> temp = mCardField->clearFields(mCardPile);
				mAnimations.insert(mAnimations.end(), temp.begin(), temp.end());
				mLeftButton->clearTextAndCallback();
				mRightButton->clearTextAndCallback();
				mCurrentStatus = ClearField;
				mCurrentDefender = Enemy;
			}
			mPlayerAction = AI::None;
			break;
		}
		case EnemyTurn:
		{
			elapsedTime += dt;
			if (elapsedTime < sf::seconds(1.5f))
			{
				break;
			}
			Card *result;
			CardField::Type type = (mCurrentDefender == Enemy) ? CardField::Defense : CardField::Attack;
			AI::Action curAction = mAI.getCard(type, result, mEnemyCards->getCards(), mCardField->getAttackCards(), mCardField->getDefenseCards());

			if (type == CardField::Attack && curAction == AI::PutCard)
			{
				if (mCardField->getNumberOfAttackCards() >= 6 ||
					mCardField->getNumberOfAttackCards() - mCardField->getNumberOfDefenseCards() >= mPlayerCards->getNumberOfCards())
				{
					curAction = AI::Pass;
				}
			}

			switch (curAction)
			{
			case AI::PutCard:
			{
				if (!mCardField->checkCard(result, type))
				{
					throw std::runtime_error("incorrect work of AI");
				}
				Card::Ptr card = mEnemyCards->getCard(*result);
				mAnimations.push_back(new Animation(card.get(), &mSceneGraph, static_cast<SceneNode *>(mCardField), type,
													card->getPosition(), mCardField->getPlace(type, card.get()), sf::seconds(0.5f)));
				mSceneGraph.attachChild(std::move(card));
				if (mTakesConfirmation)
				{
					elapsedTime = sf::Time::Zero;
				}
				else
				{
					if (type == CardField::Attack)
					{
						mLeftButton->setText("Take it");
						mLeftButton->setCallback([this]()
												 { mPlayerAction = AI::TakeCards; });
						mCurrentStatus = PlayerTurn;
					}
					else if (mPlayerCards->getNumberOfCards() == 0)
					{
						elapsedTime = sf::Time::Zero;
					}
					else
					{
						mRightButton->setText("Pass");
						mRightButton->setCallback([this]()
												  { mPlayerAction = AI::Pass; });
						mCurrentStatus = PlayerTurn;
					}
				}
				break;
			}
			case AI::TakeCards:
				mMainLabel->setText("Enemy take cards");
				mLabelElapsedTime = sf::Time::Zero;
				if (mEnemyCards->getNumberOfCards() == 0)
				{
					std::vector<Animation *> temp = mCardField->clearFields(mEnemyCards);
					mAnimations.insert(mAnimations.end(), temp.begin(), temp.end());
					mCurrentDefender = Enemy;
					mCurrentStatus = ClearField;
				}
				else
				{
					mTakesConfirmation = true;
					mRightButton->setText("Pass");
					mRightButton->setCallback([this]()
											  { mPlayerAction = AI::Pass; });
					mCurrentStatus = PlayerTurn;
				}
				break;
			case AI::Pass:
				if (mTakesConfirmation)
				{
					std::vector<Animation *> temp = mCardField->clearFields(mPlayerCards);
					mAnimations.insert(mAnimations.end(), temp.begin(), temp.end());
					mCurrentDefender = Player;
					mCurrentStatus = ClearField;
					mTakesConfirmation = false;
				}
				else
				{

					mLeftButton->setText("Take it");
					mLeftButton->setCallback([this]()
											 { mPlayerAction = AI::TakeCards; });
					mRightButton->setText("Discard");
					mRightButton->setCallback([this]()
											  { mPlayerAction = AI::DiscardCards; });
					mAvailableSelection = false;
					mCurrentStatus = PlayerTurn;
				}
				break;
			case AI::DiscardCards:
				std::vector<Animation *> temp = mCardField->clearFields(mCardPile);
				mAnimations.insert(mAnimations.end(), temp.begin(), temp.end());
				mCurrentStatus = ClearField;
				mCurrentDefender = Player;
			}

			break;
		}
		case ClearField:
		{
			// deal cards
			int commonCounter = 0;
			if (mCurrentDefender == Player)
			{
				for (int count = mPlayerCards->getNumberOfCards(); count < 6; count++)
				{
					if (mCardDeck->isEmpty())
					{
						break;
					}
					Card::Ptr playerCard = mCardDeck->popCard();
					mAnimations.push_back(new Animation(playerCard.get(), &mSceneGraph, static_cast<SceneNode *>(mPlayerCards),
														playerCard->getPosition(), mPlayerCards->getPosition(), sf::seconds(1.f)));
					mAnimations[mAnimations.size() - 1]->setDelayTime(sf::seconds(commonCounter * 0.2f));
					mSceneGraph.attachChild(std::move(playerCard));
					commonCounter++;
				}
				for (int count = mEnemyCards->getNumberOfCards(); count < 6; count++)
				{
					if (mCardDeck->isEmpty())
					{
						break;
					}
					Card::Ptr enemyCard = mCardDeck->popCard();
					mAnimations.push_back(new Animation(enemyCard.get(), &mSceneGraph, static_cast<SceneNode *>(mEnemyCards),
														enemyCard->getPosition(), mEnemyCards->getPosition(), sf::seconds(1.f)));
					mAnimations[mAnimations.size() - 1]->setDelayTime(sf::seconds(commonCounter * 0.2f));
					mSceneGraph.attachChild(std::move(enemyCard));
					commonCounter++;
				}
			}
			else
			{
				for (int count = mEnemyCards->getNumberOfCards(); count < 6; count++)
				{
					if (mCardDeck->isEmpty())
					{
						break;
					}
					Card::Ptr enemyCard = mCardDeck->popCard();
					mAnimations.push_back(new Animation(enemyCard.get(), &mSceneGraph, static_cast<SceneNode *>(mEnemyCards),
														enemyCard->getPosition(), mEnemyCards->getPosition(), sf::seconds(1.f)));
					mAnimations[mAnimations.size() - 1]->setDelayTime(sf::seconds(commonCounter * 0.2f));
					mSceneGraph.attachChild(std::move(enemyCard));
					commonCounter++;
				}
				for (int count = mPlayerCards->getNumberOfCards(); count < 6; count++)
				{
					if (mCardDeck->isEmpty())
					{
						break;
					}
					Card::Ptr playerCard = mCardDeck->popCard();
					mAnimations.push_back(new Animation(playerCard.get(), &mSceneGraph, static_cast<SceneNode *>(mPlayerCards),
														playerCard->getPosition(), mPlayerCards->getPosition(), sf::seconds(1.f)));
					mAnimations[mAnimations.size() - 1]->setDelayTime(sf::seconds(commonCounter * 0.2f));
					mSceneGraph.attachChild(std::move(playerCard));
					commonCounter++;
				}
			}

			if (mPlayerCards->getNumberOfCards() == 0 || mEnemyCards->getNumberOfCards() == 0)
			{
				if (mPlayerCards->getNumberOfCards() == 0 && mEnemyCards->getNumberOfCards() != 0)
				{
					getContext().gameStatus->setCurrentStatus(GameStatus::PlayerWon);
					requestStackPush(States::GameEnd);
				}
				else if (mPlayerCards->getNumberOfCards() != 0 && mEnemyCards->getNumberOfCards() == 0)
				{
					getContext().gameStatus->setCurrentStatus(GameStatus::EnemyWon);
					requestStackPush(States::GameEnd);
				}
				else
				{
					getContext().gameStatus->setCurrentStatus(GameStatus::Draw);
					requestStackPush(States::GameEnd);
				}
			}
			else
			{
				if (mCurrentDefender == Player)
				{
					mCurrentStatus = EnemyTurn;
					mMainLabel->setText("You are defender");
				}
				else
				{
					mCurrentStatus = PlayerTurn;
					mMainLabel->setText("Enemy is defender");
				}
			}
			mLabelElapsedTime = sf::Time::Zero;
			mAvailableSelection = true;
			break;
		}
		case DealCards:
		{
			Card::Rank playerMinTrump, enemyMinTrump;

			if (mPlayerCards->getMinimumTrump(enemyMinTrump))
			{
				if (mPlayerCards->getMinimumTrump(playerMinTrump) && (playerMinTrump < enemyMinTrump))
				{
					mCurrentDefender = Enemy;
					mCurrentStatus = PlayerTurn;
					mMainLabel->setText("Your turn");
				}
				else
				{
					mCurrentDefender = Player;
					mCurrentStatus = EnemyTurn;
					mMainLabel->setText("the enemy's turn");
				}
			}
			else
			{
				mCurrentDefender = Player;
				mCurrentStatus = EnemyTurn;
				mMainLabel->setText("the enemy's turn");
			}
			mLabelElapsedTime = sf::Time::Zero;
			break;
		}
		}
	}
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
		mGUIContainer->handleEvent(event);
		if (mAvailableSelection)
		{
			mPlayerCards->handleEvent(event);
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				if (mPlayerCards->hasSelection())
				{
					mPlayerAction = AI::PutCard;
				}
			}
		}
	}
	return true;
}