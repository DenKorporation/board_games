#include "States/MultiplayerConnectionState.h"
#include "GUI/Button.h"
#include "GUI/Label.h"
#include "GUI/List.h"
#include "GUI/OnlineGameItem.h"
#include "Engine/Resource/ResourceHolder.hpp"
#include "Engine/SpriteNode.h"
#include "Engine/ShapeNode.h"
#include "ServerService.h"
#include "Utility.h"
#include "Game/GameInfo.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

MultiplayerConnectionState::MultiplayerConnectionState(StateStack &stack, Context context)
	: State(stack, context),
	  timeSinceLastUpdate(sf::seconds(0.f)),
	  mInitListThread(&MultiplayerConnectionState::initConnectionList, this),
	  mIsThreadRunning(false)
{
	unsigned int winX = context.window->getSize().x, winY = context.window->getSize().y;
	sf::Vector2f buttonSize(winX / 5.f, winY / 10.f);

	mSceneGraph.setPosition(winX / 2.f, winY / 2.f);

	sf::Texture &texture = context.textures->get(Textures::ID(Textures::Global::Background));
	SpriteNode::Ptr backgroundSprite(new SpriteNode(texture));
	backgroundSprite->setScale((float)winX / texture.getSize().x, (float)winY / texture.getSize().y);
	mSceneGraph.attachChild(std::move(backgroundSprite));

	GUI::Container::Ptr GUIContainer(new GUI::Container);
	mGUIContainer = GUIContainer.get();
	mSceneGraph.attachChild(std::move(GUIContainer));

	GUI::Label::Ptr label(new GUI::Label("Open rooms", *context.fonts, Fonts::Main));
	label->setFontSize((unsigned int)winY / 16);
	label->setOutlineThickness(2.f);
	label->setFillColor(sf::Color::White);
	label->setPosition(-(winX / 4.f), -3.f * buttonSize.y);
	mGUIContainer->attachChild(std::move(label));

	GUI::Button::Ptr createRoomButton(new GUI::Button(*context.fonts, *context.sounds));
	createRoomButton->setPosition(-(winX / 4.f), -1.8f * buttonSize.y);
	createRoomButton->setSize(buttonSize);
	createRoomButton->setText("Create");
	createRoomButton->setNormalStyle(GUI::Button::Style((unsigned int)buttonSize.y / 2u, sf::Color::White, sf::Color(105, 105, 105),
														sf::Color::Black, 3.f));
	createRoomButton->setSelectedStyle(GUI::Button::Style((unsigned int)buttonSize.y / 2u, sf::Color::Red, sf::Color(105, 105, 105),
														  sf::Color::Black, 3.f));
	createRoomButton->setCallback([this]()
								  { requestStackPush(States::CreatingRoom); });

	mGUIContainer->attachChild(std::move(createRoomButton));

	GUI::Button::Ptr connectButton(new GUI::Button(*context.fonts, *context.sounds));
	connectButton->setPosition(-(winX / 4.f), -0.6f * buttonSize.y);
	connectButton->setSize(buttonSize);
	connectButton->setText("Connect");
	connectButton->setNormalStyle(GUI::Button::Style((unsigned int)buttonSize.y / 2u, sf::Color::White, sf::Color(105, 105, 105),
													 sf::Color::Black, 3.f));
	connectButton->setSelectedStyle(GUI::Button::Style((unsigned int)buttonSize.y / 2u, sf::Color::Red, sf::Color(105, 105, 105),
													   sf::Color::Black, 3.f));
	connectButton->setCallback([this]()
							   {
								std::any data;
								{
									sf::Lock lock(mMutex);
									data = mConnectionList->getSelectedData();
								}
								
								if (data.has_value())
								{
									getContext().gameInfo->setGameDescription(std::any_cast<GameDescription>(data));
									requestStackPush(States::ConnectionWait);
								} });

	mGUIContainer->attachChild(std::move(connectButton));

	GUI::Button::Ptr refreshButton(new GUI::Button(*context.fonts, *context.sounds));
	refreshButton->setPosition(-(winX / 4.f), 0.6f * buttonSize.y);
	refreshButton->setSize(buttonSize);
	refreshButton->setText("Refresh list");
	refreshButton->setNormalStyle(GUI::Button::Style((unsigned int)buttonSize.y / 2u, sf::Color::White, sf::Color(105, 105, 105),
													 sf::Color::Black, 3.f));
	refreshButton->setSelectedStyle(GUI::Button::Style((unsigned int)buttonSize.y / 2u, sf::Color::Red, sf::Color(105, 105, 105),
													   sf::Color::Black, 3.f));
	refreshButton->setCallback([this]()
							   {
								   sf::Lock lock(mMutex);
								   if (!mIsThreadRunning)
								   {
									   mIsThreadRunning = true;
									   mInitListThread.launch();
								   } });

	mGUIContainer->attachChild(std::move(refreshButton));

	GUI::Button::Ptr backButton(new GUI::Button(*context.fonts, *context.sounds));
	backButton->setPosition(-(winX / 4.f), 1.8f * buttonSize.y);
	backButton->setSize(buttonSize);
	backButton->setText("Back");
	backButton->setNormalStyle(GUI::Button::Style((unsigned int)buttonSize.y / 2u, sf::Color::White, sf::Color(105, 105, 105),
												  sf::Color::Black, 3.f));
	backButton->setSelectedStyle(GUI::Button::Style((unsigned int)buttonSize.y / 2u, sf::Color::Red, sf::Color(105, 105, 105),
													sf::Color::Black, 3.f));
	backButton->setCallback([this]()
							{
		requestStackPop();
		requestStackPush(States::Menu); });

	mGUIContainer->attachChild(std::move(backButton));

	GUI::List::Ptr connectionList(new GUI::List(sf::Vector2f(winX * 0.35f, winY), GUI::Offset(0, 0)));
	connectionList->setPosition(0.175f * winX, -(winY / 2.f));
	mConnectionList = connectionList.get();
	mGUIContainer->attachChild(std::move(connectionList));

	mConnectionList->setFillColor(sf::Color(105, 105, 105, 220));
	mConnectionList->setOutlineThickness(3.f);
	mConnectionList->setOutlineColor(sf::Color::Black);

	mIsThreadRunning = true;
	mInitListThread.launch();
}

void MultiplayerConnectionState::draw()
{
	sf::Lock lock(mMutex);
	sf::RenderWindow &window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mSceneGraph);
}

bool MultiplayerConnectionState::update(sf::Time dt)
{
	sf::Lock lock(mMutex);
	timeSinceLastUpdate += dt;
	if (timeSinceLastUpdate > sf::seconds(5.f) && !mIsThreadRunning)
	{
		mIsThreadRunning = true;
		mInitListThread.launch();
	}
	return true;
}

bool MultiplayerConnectionState::handleEvent(const sf::Event &event)
{
	sf::Lock lock(mMutex);
	mGUIContainer->handleEvent(event);
	return true;
}

void MultiplayerConnectionState::initConnectionList()
{
	json query;
	query["Type"] = "List";
	ServerService service;
	service.Connect();
	service.Send(query);
	json data = service.Receive();
	service.Disconnect();

	sf::Lock lock(mMutex);

	unsigned int winX = getContext().window->getSize().x, winY = getContext().window->getSize().y;
	sf::Vector2f buttonSize(winX / 5.f, winY / 10.f);

	GUI::OnlineGameItem::Style normalStyle(GUI::OnlineGameItem::Style((unsigned int)buttonSize.y / 3u, sf::Color::White, (unsigned int)buttonSize.y / 6u, sf::Color::White, sf::Color(105, 105, 105),
																	  sf::Color::Black, 3.f));
	GUI::OnlineGameItem::Style selectedStyle(GUI::OnlineGameItem::Style((unsigned int)buttonSize.y / 3u, sf::Color::Green, (unsigned int)buttonSize.y / 6u, sf::Color::Green, sf::Color(105, 105, 105),
																		sf::Color::Black, 3.f));
	GUI::OnlineGameItem::Style hoverStyle(GUI::OnlineGameItem::Style((unsigned int)buttonSize.y / 3u, sf::Color::Red, (unsigned int)buttonSize.y / 6u, sf::Color::Red, sf::Color(105, 105, 105),
																	 sf::Color::Black, 3.f));

	mConnectionList->detachAllListItem();

	for (auto element : data)
	{
		GUI::OnlineGameItem::Ptr listItem(new GUI::OnlineGameItem(GUI::Offset(20, 20), *getContext().fonts, *getContext().sounds));

		GameDescription game = GameDescription();
		game.Name = sf::String(utf8_to_wstring(element["Name"]));
		game.Id = element["Id"];
		game.Count = element["Count"];

		listItem->setData(game);
		listItem->setSize(buttonSize * 1.5f);
		listItem->setNormalStyle(normalStyle);
		listItem->setHoverStyle(hoverStyle);
		listItem->setSelectedStyle(selectedStyle);
		mConnectionList->pack(std::move(listItem));
	}

	timeSinceLastUpdate = sf::seconds(0.f);
	mIsThreadRunning = false;
}

MultiplayerConnectionState::~MultiplayerConnectionState()
{
	mInitListThread.terminate();
}
