#include "States/ConnectionWaitState.h"
#include "GUI/Container.h"
#include "GUI/Button.h"
#include "GUI/Label.h"
#include "Engine/ShapeNode.h"
#include "Engine/Resource/ResourceHolder.hpp"
#include "Engine/Resource/ResourceIdentifiers.h"
#include "Game/GameStatus.h"
#include "Utility.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <string>

ConnectionWaitState::ConnectionWaitState(StateStack &stack, Context context)
	: State(stack, context),
	  mSceneGraph(),
	  mGUIContainer()
{
	unsigned int winX = context.window->getSize().x, winY = context.window->getSize().y;
	sf::Vector2f menuSize = sf::Vector2f(winX / 3.f, winY / 4.f);

	mSceneGraph.setPosition(winX / 2.f, winY / 2.f);

	ShapeNode::Ptr menuBackground(new ShapeNode(menuSize));
	menuBackground->setFillColor(sf::Color(105, 105, 105));
	menuBackground->setOutlineColor(sf::Color::Black);
	menuBackground->setOutlineThickness(3.f);
	mSceneGraph.attachChild(std::move(menuBackground));

	GUI::Container::Ptr GUIMenu(new GUI::Container());
	mGUIContainer = GUIMenu.get();
	mSceneGraph.attachChild(std::move(GUIMenu));

	std::string header = "Waiting for the player(" + std::to_string(getContext().gameStatus->getGameDescription().Count) + "/2)";
	GUI::Label::Ptr label(new GUI::Label(header, *context.fonts, Fonts::Main));
	label->setFontSize((unsigned int)menuSize.y / 6);
	label->setPosition(0.f, -menuSize.y / 3.f);
	mGUIContainer->attachChild(std::move(label));

	GUI::Label::Ptr nameRoomLabel(new GUI::Label(getContext().gameStatus->getGameDescription().Name, *context.fonts, Fonts::Russian));
	nameRoomLabel->setFontSize((unsigned int)menuSize.y / 7);
	nameRoomLabel->setPosition(0.f, -(menuSize.y / 10.f));
	mGUIContainer->attachChild(std::move(nameRoomLabel));

	GUI::Label::Ptr IdRoomLabel(new GUI::Label(getContext().gameStatus->getGameDescription().Id, *context.fonts, Fonts::Russian));
	IdRoomLabel->setFontSize((unsigned int)menuSize.y / 12);
	IdRoomLabel->setPosition(0.f, menuSize.y / 10.f);
	mGUIContainer->attachChild(std::move(IdRoomLabel));

	GUI::Button::Ptr cancelButton(new GUI::Button(*context.fonts, *context.sounds));
	cancelButton->setPosition(0.f, menuSize.y / 3.f);
	cancelButton->setSize(sf::Vector2f(menuSize.x / 3.f, menuSize.y / 5.f));
	cancelButton->setText("Cancel");
	cancelButton->setNormalStyle(GUI::Button::Style(menuSize.y / 6.f, sf::Color::White, sf::Color(105, 105, 105),
													sf::Color::Black, 3.f));
	cancelButton->setSelectedStyle(GUI::Button::Style(menuSize.y / 6.f, sf::Color::Black, sf::Color::White,
													  sf::Color::Black, 3.f));
	cancelButton->setCallback([this]()
							  { requestStackPop(); });
	mGUIContainer->attachChild(std::move(cancelButton));
}

void ConnectionWaitState::draw()
{
	sf::RenderWindow &window = *getContext().window;
	window.draw(mSceneGraph);
}

bool ConnectionWaitState::update(sf::Time dt)
{
	return true;
}

bool ConnectionWaitState::handleEvent(const sf::Event &event)
{
	mGUIContainer->handleEvent(event);
	return false;
}
