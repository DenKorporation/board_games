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

	unsigned int winX = context.window->getSize().x, winY = context.window->getSize().y;
	sf::Vector2u cardDefaultSize = mTextures.get(Textures::ID(Textures::Global::ReverseFace)).getSize();
	sf::Vector2f cardCurrentSize = sf::Vector2f((float)winX * cardDefaultSize.x / 1920, (float)winY * cardDefaultSize.y / 1080);

	sf::Texture &texture = context.textures->get(Textures::ID(Textures::Global::Background));
	SpriteNode::Ptr backgroundSprite(new SpriteNode(texture));
	backgroundSprite->setScale((float)winX / texture.getSize().x, (float)winY / texture.getSize().y);
	backgroundSprite->setPosition(winX / 2.f, winY / 2.f);
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
			card->setScale(cardCurrentSize.x / cardDefaultSize.x, cardCurrentSize.y / cardDefaultSize.y);
			mCardDeck->pushCard(std::move(card));
		}
	}
	mCardDeck->shuffle();
	SpriteNode::Ptr suitSprite(new SpriteNode(mTextures.get(Textures::ID((Textures::Global::ID)mCardDeck->getTrump(), Textures::Local::Main))));
	suitSprite->setScale(cardCurrentSize.x / cardDefaultSize.x, cardCurrentSize.y / cardDefaultSize.y);
	mCardDeck->attachChild(std::move(suitSprite));
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
	return true;
}