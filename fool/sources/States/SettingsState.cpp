#include "States/SettingsState.h"
#include "GUI/Button.h"
#include "GUI/Label.h"
#include "Utility.h"
#include "ServerService.h"
#include "Engine/Resource/ResourceHolder.hpp"
#include "Engine/SpriteNode.h"
#include "Engine/ShapeNode.h"
#include "Game/PlayerInfo.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

SettingsState::SettingsState(StateStack &stack, Context context)
	: State(stack, context),
	  mSceneGraph(),
	  mMutex(),
	  mConnectionThread(&SettingsState::checkConnection, this),
	  mIsThreadRunning(false)
{
	unsigned int winX = context.window->getSize().x, winY = context.window->getSize().y;
	sf::Vector2f blockSize(winX / 1.3f, winY * 0.95f);

	mSceneGraph.setPosition(winX / 2.f, 0.f);

	sf::Texture &texture = context.textures->get(Textures::ID(Textures::Global::Background));
	SpriteNode::Ptr backgroundSprite(new SpriteNode(texture));
	backgroundSprite->setScale((float)winX / texture.getSize().x, (float)winY / texture.getSize().y);
	backgroundSprite->setPosition(0.f, winY / 2.f);
	mSceneGraph.attachChild(std::move(backgroundSprite));

	ShapeNode::Ptr statisticBackground(new ShapeNode(blockSize));
	statisticBackground->setPosition(0.f, winY / 2.f);
	statisticBackground->setFillColor(sf::Color(105, 105, 105, 220));
	statisticBackground->setOutlineThickness(3.f);
	statisticBackground->setOutlineColor(sf::Color::Black);

	GUI::Container::Ptr GUIContainer(new GUI::Container);
	GUIContainer->setPosition(0.f, -blockSize.y / 2.f);
	mGUIContainer = GUIContainer.get();
	statisticBackground->attachChild(std::move(GUIContainer));
	mSceneGraph.attachChild(std::move(statisticBackground));

	GUI::Label::Ptr label(new GUI::Label("Settings", *context.fonts, Fonts::Main));
	label->setFontSize((unsigned int)blockSize.y / 10);
	label->setOutlineThickness(3.f);
	label->setFillColor(sf::Color(200, 200, 200));
	label->setPosition(0.f, blockSize.y * 0.08f);
	mGUIContainer->attachChild(std::move(label));

	GUI::Label::Ptr nameText(new GUI::Label("Name:", *context.fonts, Fonts::Main));
	nameText->setFontSize((unsigned int)blockSize.y / 13);
	nameText->setOutlineThickness(1.f);
	nameText->setFillColor(sf::Color(200, 200, 200));
	nameText->setPosition(-0.47f * blockSize.x + nameText->getBounds().width / 2.f, blockSize.y * 0.17f);

	GUI::InputText::Style NameActiveStyle = GUI::InputText::Style((unsigned int)blockSize.y / 20, sf::Color::Black,
																  sf::Color::White, sf::Color::Black, 2.f);
	GUI::InputText::Style NameNotActiveStyle = GUI::InputText::Style((unsigned int)blockSize.y / 20, sf::Color::Black,
																	 sf::Color(200, 200, 200), sf::Color::Black, 2.f);

	GUI::InputText::Ptr nameInput(new GUI::InputText(*context.fonts, Fonts::Russian));
	nameInput->setPosition(nameText->getBounds().width / 2.f, blockSize.y * 0.17f);
	nameInput->setSize(sf::Vector2f(blockSize.x * 0.9f - nameText->getBounds().width, blockSize.y / 13.f));
	nameInput->setAlignment(GUI::InputText::Left);
	nameInput->setActiveStyle(NameActiveStyle);
	nameInput->setNotActiveStyle(NameNotActiveStyle);
	nameInput->setText(getContext().playerInfo->getName());
	nameInput->setOnEnterAction([this]()
								{ getContext().playerInfo->setName(mNameInput->getString()); });
	mNameInput = nameInput.get();
	mGUIContainer->attachChild(std::move(nameText));
	mGUIContainer->attachChild(std::move(nameInput));

	GUI::Label::Ptr serverLabel(new GUI::Label("Server", *context.fonts, Fonts::Main));
	serverLabel->setFontSize((unsigned int)blockSize.y / 12);
	serverLabel->setOutlineThickness(2.f);
	serverLabel->setFillColor(sf::Color(200, 200, 200));
	serverLabel->setPosition(0.f, blockSize.y * 0.3f);
	mGUIContainer->attachChild(std::move(serverLabel));

	GUI::Label::Ptr ipText(new GUI::Label("IP", *context.fonts, Fonts::Main));
	ipText->setFontSize((unsigned int)blockSize.y / 15);
	ipText->setOutlineThickness(1.f);
	ipText->setFillColor(sf::Color(200, 200, 200));
	ipText->setPosition(-0.45f * blockSize.x + ipText->getBounds().width / 2.f, blockSize.y * 0.4f);
	mGUIContainer->attachChild(std::move(ipText));

	GUI::InputText::Style IPPortActiveStyle = GUI::InputText::Style((unsigned int)blockSize.y / 23, sf::Color::Black,
																	sf::Color::White, sf::Color::Black, 2.f);
	GUI::InputText::Style IPPortNotActiveStyle = GUI::InputText::Style((unsigned int)blockSize.y / 23, sf::Color::Black,
																	   sf::Color(200, 200, 200), sf::Color::Black, 2.f);

	GUI::InputText::Ptr ipInput(new GUI::InputText(*context.fonts, Fonts::Main));
	ipInput->setPosition(-(blockSize.x * 0.25f), blockSize.y * 0.47f);
	ipInput->setSize(sf::Vector2f(blockSize.x * 0.4f, blockSize.y / 15.f));
	ipInput->setOnInputAction([](sf::Uint32 code)
							  { return (code >= 48) && (code <= 57) || (code == 46); });
	ipInput->setOnEnterAction([this]()
							  { ServerService::setIp(mIpInput->getString()); });
	ipInput->setText(ServerService::getIp());
	ipInput->setAlignment(GUI::InputText::Left);
	ipInput->setActiveStyle(IPPortActiveStyle);
	ipInput->setNotActiveStyle(IPPortNotActiveStyle);
	mIpInput = ipInput.get();
	mGUIContainer->attachChild(std::move(ipInput));

	GUI::Label::Ptr portText(new GUI::Label("Port", *context.fonts, Fonts::Main));
	portText->setFontSize((unsigned int)blockSize.y / 15);
	portText->setOutlineThickness(1.f);
	portText->setFillColor(sf::Color(200, 200, 200));
	portText->setPosition(-0.45f * blockSize.x + portText->getBounds().width / 2.f, blockSize.y * 0.55f);
	mGUIContainer->attachChild(std::move(portText));

	GUI::InputText::Ptr portInput(new GUI::InputText(*context.fonts, Fonts::Main));
	portInput->setPosition(-(blockSize.x * 0.25f), blockSize.y * 0.62f);
	portInput->setSize(sf::Vector2f(blockSize.x * 0.4f, blockSize.y / 15.f));
	portInput->setOnInputAction([](sf::Uint32 code)
								{ return (code >= 48) && (code <= 57); });
	portInput->setOnEnterAction([this]()
								{ 	int number = std::stoi(mPortInput->getString().toAnsiString());
									if(number >=0 && number <= 65535)
										ServerService::setPort((unsigned short)number); });
	portInput->setText(std::to_string(ServerService::getPort()));
	portInput->setAlignment(GUI::InputText::Left);
	portInput->setActiveStyle(IPPortActiveStyle);
	portInput->setNotActiveStyle(IPPortNotActiveStyle);
	mPortInput = portInput.get();
	mGUIContainer->attachChild(std::move(portInput));

	GUI::Button::Ptr checkButton(new GUI::Button(*context.fonts, *context.sounds));
	checkButton->setSize(sf::Vector2f(blockSize.x / 5.f, blockSize.y / 10.f));
	checkButton->setPosition(blockSize.x / 9.f, blockSize.y * 0.45f);
	checkButton->setText("Check");
	checkButton->setNormalStyle(GUI::Button::Style((unsigned int)blockSize.y / 20, sf::Color::White, sf::Color(105, 105, 105),
												   sf::Color::Black, 3.f));
	checkButton->setSelectedStyle(GUI::Button::Style((unsigned int)blockSize.y / 20, sf::Color::Red, sf::Color(105, 105, 105),
													 sf::Color::Black, 3.f));
	checkButton->setCallback([this]()
							 { 	sf::Lock lock(mMutex);
								if (!mIsThreadRunning)
								{
									mIsThreadRunning = true;
									mConnectionThread.launch();
								} });

	mGUIContainer->attachChild(std::move(checkButton));

	GUI::Label::Ptr replyLabel(new GUI::Label("", *context.fonts, Fonts::Main));
	replyLabel->setPosition(blockSize.x * 0.3f, blockSize.y * 0.45f);
	replyLabel->setFontSize((unsigned int)blockSize.y / 15);
	replyLabel->setOutlineThickness(2.f);
	replyLabel->setFillColor(sf::Color::Green);
	mReplyLabel = replyLabel.get();
	mGUIContainer->attachChild(std::move(replyLabel));

	GUI::Button::Ptr backButton(new GUI::Button(*context.fonts, *context.sounds));
	backButton->setPosition(-(blockSize.x / 5.f), blockSize.y * 0.92f);
	backButton->setSize(sf::Vector2f(blockSize.x / 4.f, blockSize.y / 10.f));
	backButton->setText("Back");
	backButton->setNormalStyle(GUI::Button::Style((unsigned int)blockSize.y / 20, sf::Color::White, sf::Color(105, 105, 105),
												  sf::Color::Black, 3.f));
	backButton->setSelectedStyle(GUI::Button::Style((unsigned int)blockSize.y / 20, sf::Color::Red, sf::Color(105, 105, 105),
													sf::Color::Black, 3.f));
	backButton->setCallback([this]()
							{
		requestStackPop();
		requestStackPush(States::Menu); });

	mGUIContainer->attachChild(std::move(backButton));

	GUI::Button::Ptr confirmButton(new GUI::Button(*context.fonts, *context.sounds));
	confirmButton->setPosition((blockSize.x / 5.f), blockSize.y * 0.92f);
	confirmButton->setSize(sf::Vector2f(blockSize.x / 4.f, blockSize.y / 10.f));
	confirmButton->setText("Confirm");
	confirmButton->setNormalStyle(GUI::Button::Style((unsigned int)blockSize.y / 20, sf::Color::White, sf::Color(105, 105, 105),
													 sf::Color::Black, 3.f));
	confirmButton->setSelectedStyle(GUI::Button::Style((unsigned int)blockSize.y / 20, sf::Color::Red, sf::Color(105, 105, 105),
													   sf::Color::Black, 3.f));
	confirmButton->setCallback([this]()
							   { 	getContext().playerInfo->setName(mNameInput->getString()); 
							   		ServerService::setIp(mIpInput->getString()); 
							   		int number = std::stoi(mPortInput->getString().toAnsiString());
									if(number >=0 && number <= 65535)
										ServerService::setPort((unsigned short)number); });

	mGUIContainer->attachChild(std::move(confirmButton));
}

void SettingsState::draw()
{
	sf::Lock lock(mMutex);
	sf::RenderWindow &window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mSceneGraph);
}

bool SettingsState::update(sf::Time dt)
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event &event)
{
	sf::Lock lock(mMutex);
	mGUIContainer->handleEvent(event);
	return true;
}

void SettingsState::checkConnection()
{
	std::string ip;
	unsigned short port;
	{
		sf::Lock lock(mMutex);
		mReplyLabel->setText("");
		ip = ServerService::getIp();
		port = ServerService::getPort();
	}

	json query;
	query["Type"] = "Test";
	ServerService service;
	if (service.Connect(ip, port))
	{
		service.Send(query);
		try
		{
			json reply = service.Receive();
			if (reply["Type"] == "Test" && reply["Status"] == "Ok")
			{
				sf::Lock lock(mMutex);
				mReplyLabel->setText("Done");
				mReplyLabel->setFillColor(sf::Color::Green);
			}
			else
			{
				sf::Lock lock(mMutex);
				mReplyLabel->setText("Fail");
				mReplyLabel->setFillColor(sf::Color::Red);
			}
		}
		catch (json::exception)
		{
			sf::Lock lock(mMutex);
			mReplyLabel->setText("Fail");
			mReplyLabel->setFillColor(sf::Color::Red);
		}
		service.Disconnect();
	}
	else
	{
		sf::Lock lock(mMutex);
		mReplyLabel->setText("Fail");
		mReplyLabel->setFillColor(sf::Color::Red);
	}
	sf::Lock lock(mMutex);
	mIsThreadRunning = false;
}

SettingsState::~SettingsState()
{
	mConnectionThread.terminate();
}