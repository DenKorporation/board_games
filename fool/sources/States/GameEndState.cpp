#include "States/GameEndState.h"
#include "GUI/Container.h"
#include "GUI/Button.h"
#include "GUI/Label.h"
#include "Engine/ShapeNode.h"
#include "Engine/Resource/ResourceHolder.hpp"
#include "Engine/Resource/ResourceIdentifiers.h"
#include "Utility.h"
#include "Game/GameStatus.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <stdexcept>

GameEndState::GameEndState(StateStack &stack, Context context)
	: State(stack, context),
	  mSceneGraph(),
	  mGUIContainer()
{
	unsigned int winX = context.window->getSize().x, winY = context.window->getSize().y;
	sf::Vector2f menuSize = sf::Vector2f(winX / 2.f, winY / 3.f);

	mSceneGraph.setPosition(winX / 2.f, winY / 2.f);

	ShapeNode::Ptr menuBackground(new ShapeNode(menuSize));
	menuBackground->setFillColor(sf::Color(105, 105, 105, 220));
	menuBackground->setOutlineColor(sf::Color::Black);
	menuBackground->setOutlineThickness(3.f);
	mSceneGraph.attachChild(std::move(menuBackground));

	GUI::Container::Ptr GUIMenu(new GUI::Container());
	mGUIContainer = GUIMenu.get();
	mSceneGraph.attachChild(std::move(GUIMenu));

	GUI::Label::Ptr label(new GUI::Label("", *context.fonts, Fonts::Main));
	Musics::ID curTheme;
	switch (getContext().gameStatus->getCurrentStatus())
	{
	case GameStatus::Draw:
		label->setText("Draw");
		curTheme = Musics::DrawTheme;
		break;
	case GameStatus::PlayerWon:
		label->setText("You win");
		curTheme = Musics::VictoryTheme;
		break;
	case GameStatus::EnemyWon:
		label->setText("Game Over");
		curTheme = Musics::LoseTheme;
		break;
	}
	if (!mGameEndTheme.openFromFile(getFilePath(curTheme)))
	{
		throw std::runtime_error("Fail to open music mGameEndTheme.openFromFile : " + getFilePath(curTheme));
	}

	label->setFontSize((unsigned int)menuSize.y / 3);
	label->setPosition(0.f, -menuSize.y / 4.f);
	mGUIContainer->attachChild(std::move(label));

	GUI::Button::Ptr newGameButton(new GUI::Button(*context.fonts, *context.sounds));
	newGameButton->setPosition(-menuSize.x / 4.f, menuSize.y / 4.f);
	newGameButton->setSize(sf::Vector2f(menuSize.x / 3.f, menuSize.y / 4.f));
	newGameButton->setText("NEW GAME");
	newGameButton->setNormalStyle(GUI::Button::Style(menuSize.y / 10.f, sf::Color::Red, sf::Color(105, 105, 105),
													 sf::Color::Black, 3.f));
	newGameButton->setSelectedStyle(GUI::Button::Style(menuSize.y / 10.f, sf::Color::Red, sf::Color::White,
													   sf::Color::Black, 3.f));
	newGameButton->setCallback([this]()
							   {
		requestStackClear();
		requestStackPush(States::Game); });
	mGUIContainer->attachChild(std::move(newGameButton));

	GUI::Button::Ptr mainMenuButton(new GUI::Button(*context.fonts, *context.sounds));
	mainMenuButton->setPosition(menuSize.x / 4.f, menuSize.y / 4.f);
	mainMenuButton->setSize(sf::Vector2f(menuSize.x / 3.f, menuSize.y / 4.f));
	mainMenuButton->setText("TO MAIN MENU");
	mainMenuButton->setNormalStyle(GUI::Button::Style(menuSize.y / 10.f, sf::Color::Green, sf::Color(105, 105, 105),
													  sf::Color::Black, 3.f));
	mainMenuButton->setSelectedStyle(GUI::Button::Style(menuSize.y / 10.f, sf::Color::Green, sf::Color::White,
														sf::Color::Black, 3.f));
	mainMenuButton->setCallback([this]()
								{ 	requestStackClear();
									requestStackPush(States::Menu); });
	mGUIContainer->attachChild(std::move(mainMenuButton));

	mGameEndTheme.play();
}

void GameEndState::draw()
{
	sf::RenderWindow &window = *getContext().window;
	window.draw(mSceneGraph);
}

bool GameEndState::update(sf::Time dt)
{
	return false;
}

bool GameEndState::handleEvent(const sf::Event &event)
{
	mGUIContainer->handleEvent(event);
	return false;
}
