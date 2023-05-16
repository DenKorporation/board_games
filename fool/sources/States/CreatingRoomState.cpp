#include "States/CreatingRoomState.h"
#include "GUI/Container.h"
#include "GUI/Button.h"
#include "GUI/Label.h"
#include "GUI/InputText.h"
#include "Engine/ShapeNode.h"
#include "Engine/Resource/ResourceHolder.hpp"
#include "Engine/Resource/ResourceIdentifiers.h"
#include "Game/GameStatus.h"
#include "Utility.h"
#include "ServerService.h"

#include <SFML/Graphics/RenderWindow.hpp>

CreatingRoomState::CreatingRoomState(StateStack &stack, Context context)
	: State(stack, context),
	  mSceneGraph(),
	  mGUIContainer()
{
	unsigned int winX = context.window->getSize().x, winY = context.window->getSize().y;
	sf::Vector2f menuSize = sf::Vector2f(winX / 3.f, winY / 5.f);

	mSceneGraph.setPosition(winX / 2.f, winY / 2.f);

	ShapeNode::Ptr menuBackground(new ShapeNode(menuSize));
	menuBackground->setFillColor(sf::Color(105, 105, 105));
	menuBackground->setOutlineColor(sf::Color::Black);
	menuBackground->setOutlineThickness(3.f);
	mSceneGraph.attachChild(std::move(menuBackground));

	GUI::Container::Ptr GUIMenu(new GUI::Container());
	mGUIContainer = GUIMenu.get();
	mSceneGraph.attachChild(std::move(GUIMenu));

	GUI::Label::Ptr label(new GUI::Label("Enter room name", *context.fonts, Fonts::Main));
	label->setFontSize((unsigned int)menuSize.y / 5);
	label->setPosition(0.f, -menuSize.y / 3.f);
	mGUIContainer->attachChild(std::move(label));

	GUI::InputText::Ptr inputText(new GUI::InputText(*context.fonts, Fonts::Russian));
	mInputText = inputText.get();
	inputText->setFontSize((unsigned int)menuSize.y / 5);
	mGUIContainer->attachChild(std::move(inputText));

	GUI::Button::Ptr yesButton(new GUI::Button(*context.fonts, *context.sounds));
	yesButton->setPosition(-menuSize.x / 4.f, menuSize.y / 3.f);
	yesButton->setSize(sf::Vector2f(menuSize.x / 3.f, menuSize.y / 5.f));
	yesButton->setText("Create");
	yesButton->setNormalStyle(GUI::Button::Style(menuSize.y / 6.f, sf::Color::White, sf::Color(105, 105, 105),
												 sf::Color::Black, 3.f));
	yesButton->setSelectedStyle(GUI::Button::Style(menuSize.y / 6.f, sf::Color::Black, sf::Color::White,
												   sf::Color::Black, 3.f));
	yesButton->setCallback([this]()
						   { 
							if(createRoom(mInputText->getString())){
								requestStackPop();
								requestStackPush(States::ConnectionWait);
							}
							else{
								requestStackPop();
							} });
	mGUIContainer->attachChild(std::move(yesButton));

	GUI::Button::Ptr cancelButton(new GUI::Button(*context.fonts, *context.sounds));
	cancelButton->setPosition(menuSize.x / 4.f, menuSize.y / 3.f);
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

void CreatingRoomState::draw()
{
	sf::RenderWindow &window = *getContext().window;
	window.draw(mSceneGraph);
}

bool CreatingRoomState::update(sf::Time dt)
{
	return true;
}

bool CreatingRoomState::handleEvent(const sf::Event &event)
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

bool CreatingRoomState::createRoom(sf::String text)
{
	json query;
	query["Type"] = "Create";
	query["Name"] = wstring_to_utf8(text.toWideString());

	ServerService service;
	service.Connect();
	service.Send(query);
	json reply = service.Receive();
	service.Disconnect();

	if (reply["Type"] == "Create" && reply["Status"] == "Done")
	{
		GameDescription *game = &getContext().gameStatus->getGameDescription();
		game->Id = reply["Game"]["Id"];
		game->Name = sf::String(utf8_to_wstring(reply["Game"]["Name"]));
		game->Count = reply["Game"]["Count"];

		return true;
	}

	return false;
}
