#include "States/PauseState.h"
#include "GUI/Container.h"
#include "GUI/Button.h"
#include "GUI/Label.h"
#include "Engine/ShapeNode.h"
#include "Engine/Resource/ResourceHolder.hpp"
#include "Engine/Resource/ResourceIdentifiers.h"
#include "Utility.h"

#include <SFML/Graphics/RenderWindow.hpp>

PauseState::PauseState(StateStack &stack, Context context)
	: State(stack, context),
	  mSceneGraph(),
	  mGUIContainer()
{
	unsigned int winX = context.window->getSize().x, winY = context.window->getSize().y;
	sf::Vector2f menuSize = sf::Vector2f(winX / 3.f, winY / 7.f);

	mSceneGraph.setPosition(winX / 2.f, winY / 2.f);

	ShapeNode::Ptr menuBackground(new ShapeNode(menuSize));
	menuBackground->setFillColor(sf::Color(105, 105, 105));
	menuBackground->setOutlineColor(sf::Color::Black);
	menuBackground->setOutlineThickness(3.f);
	mSceneGraph.attachChild(std::move(menuBackground));

	GUI::Container::Ptr GUIMenu(new GUI::Container());
	mGUIContainer = GUIMenu.get();
	mSceneGraph.attachChild(std::move(GUIMenu));

	GUI::Label::Ptr label(new GUI::Label("Exit to the main menu?", *context.fonts, Fonts::Main));
	label->setFontSize((unsigned int)menuSize.y / 3);
	label->setPosition(0.f, -menuSize.y / 4.f);
	mGUIContainer->attachChild(std::move(label));

	GUI::Button::Ptr yesButton(new GUI::Button(*context.fonts, *context.sounds));
	yesButton->setPosition(-menuSize.x / 4.f, menuSize.y / 4.f);
	yesButton->setSize(sf::Vector2f(menuSize.x / 3.f, menuSize.y / 4.f));
	yesButton->setText("YES");
	yesButton->setNormalStyle(GUI::Button::Style(menuSize.y / 3.f, sf::Color::Red, sf::Color(105, 105, 105),
												 sf::Color::Black, 3.f));
	yesButton->setSelectedStyle(GUI::Button::Style(menuSize.y / 3.f, sf::Color::Red, sf::Color::White,
												   sf::Color::Black, 3.f));
	yesButton->setCallback([this]()
						   {
		requestStackClear();
		requestStackPush(States::Menu); });
	mGUIContainer->attachChild(std::move(yesButton));

	GUI::Button::Ptr cancelButton(new GUI::Button(*context.fonts, *context.sounds));
	cancelButton->setPosition(menuSize.x / 4.f, menuSize.y / 4.f);
	cancelButton->setSize(sf::Vector2f(menuSize.x / 3.f, menuSize.y / 4.f));
	cancelButton->setText("CANCEL");
	cancelButton->setNormalStyle(GUI::Button::Style(menuSize.y / 3.f, sf::Color::Green, sf::Color(105, 105, 105),
													sf::Color::Black, 3.f));
	cancelButton->setSelectedStyle(GUI::Button::Style(menuSize.y / 3.f, sf::Color::Green, sf::Color::White,
													  sf::Color::Black, 3.f));
	cancelButton->setCallback([this]()
							  { requestStackPop(); });
	mGUIContainer->attachChild(std::move(cancelButton));
}

void PauseState::draw()
{
	sf::RenderWindow &window = *getContext().window;
	window.draw(mSceneGraph);
}

bool PauseState::update(sf::Time dt)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event &event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			requestStackPop();
			break;
		}
	}
	mGUIContainer->handleEvent(event);
	return false;
}
