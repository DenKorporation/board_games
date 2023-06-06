#include "States/ConnectionWaitState.h"
#include "GUI/Container.h"
#include "GUI/Button.h"
#include "GUI/Label.h"
#include "Engine/ShapeNode.h"
#include "Engine/Resource/ResourceHolder.hpp"
#include "Engine/Resource/ResourceIdentifiers.h"
#include "Game/GameInfo.h"
#include "Utility.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <string>

ConnectionWaitState::ConnectionWaitState(StateStack &stack, Context context)
	: State(stack, context),
	  mSceneGraph(),
	  mGUIContainer(),
	  mCancelThread(),
	  mInitThread(),
	  mService(new ServerService()),
	  mBackgroundThread(),
	  mIsConnected(false),
	  mCancelThreadRunning(false),
	  mIsGameStarted(false)
{
	mFutureObj = mPromise.get_future();

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

	GUI::Label::Ptr label(new GUI::Label("Trying to connect...", *context.fonts, Fonts::Main));
	label->setFontSize((unsigned int)menuSize.y / 6);
	label->setPosition(0.f, -menuSize.y / 3.f);
	mHeaderLabel = label.get();
	mGUIContainer->attachChild(std::move(label));

	GUI::Label::Ptr nameRoomLabel(new GUI::Label("", *context.fonts, Fonts::Russian));
	nameRoomLabel->setFontSize((unsigned int)menuSize.y / 7);
	nameRoomLabel->setPosition(0.f, -(menuSize.y / 10.f));
	mNameLabel = nameRoomLabel.get();
	mGUIContainer->attachChild(std::move(nameRoomLabel));

	GUI::Label::Ptr IdRoomLabel(new GUI::Label("", *context.fonts, Fonts::Russian));
	IdRoomLabel->setFontSize((unsigned int)menuSize.y / 12);
	IdRoomLabel->setPosition(0.f, menuSize.y / 10.f);
	mIdLabel = IdRoomLabel.get();
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
							  {				
												std::lock_guard lock(mCancelMutex);
		if (!mCancelThreadRunning)
		{
			mCancelThreadRunning = true;
			mCancelThread = std::thread(&ConnectionWaitState::cancelAction, this);
		} });
	mGUIContainer->attachChild(std::move(cancelButton));

	mInitThread = std::thread(&ConnectionWaitState::initQuery, this);
}

ConnectionWaitState::~ConnectionWaitState()
{
	if (mInitThread.joinable())
	{
		mInitThread.join();
	}
	if (mCancelThread.joinable())
	{
		mCancelThread.join();
	}
	if (mBackgroundThread.joinable())
	{
		mPromise.set_value();
		mBackgroundThread.join();
	}
	if (mIsConnected && !mIsGameStarted)
	{
		json query;
		query["Type"] = "Disconnect";
		mService->Send(query);
	}
}

void ConnectionWaitState::draw()
{
	std::lock_guard lock(mCommonMutex);
	sf::RenderWindow &window = *getContext().window;
	window.draw(mSceneGraph);
}

bool ConnectionWaitState::update(sf::Time dt)
{
	std::lock_guard lock(mCommonMutex);
	return true;
}

bool ConnectionWaitState::handleEvent(const sf::Event &event)
{
	std::lock_guard lock(mCommonMutex);
	mGUIContainer->handleEvent(event);
	return false;
}

void ConnectionWaitState::cancelAction()
{
	json query;
	query["Type"] = "Disconnect";

	mCommonMutex.lock();
	bool isConnected = mIsConnected;
	mCommonMutex.unlock();
	if (isConnected)
	{
		mSendMutex.lock();
		mService->Send(query);
		mSendMutex.unlock();

		mCommonMutex.lock();
		mIsConnected = false;
		mCommonMutex.unlock();
	}
	std::lock_guard lock(mCommonMutex);
	std::lock_guard cancelLock(mCancelMutex);
	requestStackPop();
	mCancelThreadRunning = false;
}

void ConnectionWaitState::initQuery()
{
	json query;
	query["Type"] = "Connect";

	mCommonMutex.lock();
	query["Id"] = getContext().gameInfo->getGameDescription().Id;
	mCommonMutex.unlock();

	mConnectMutex.lock();
	bool isConnected = mService->Connect();
	mConnectMutex.unlock();

	if (isConnected)
	{
		mSendMutex.lock();
		mService->Send(query);
		mSendMutex.unlock();
		try
		{
			mReceiveMutex.lock();
			json reply = mService->Receive();
			mReceiveMutex.unlock();

			if (reply["Type"] == "Connect" && reply["Status"] == 200)
			{
				std::lock_guard lock(mCommonMutex);
				mIsConnected = true;
				getContext().gameInfo->getGameDescription().Count = reply["Game"]["Count"];
				std::string header = "Waiting for the player(" + std::to_string(getContext().gameInfo->getGameDescription().Count) + "/2)";
				mHeaderLabel->setFillColor(sf::Color::White);
				mHeaderLabel->setText(header);
				mNameLabel->setUtf8Text(getContext().gameInfo->getGameDescription().Name);
				mIdLabel->setText(getContext().gameInfo->getGameDescription().Id);

				mBackgroundThread = std::thread(&ConnectionWaitState::backgroundAction, this, std::move(mFutureObj));
			}
			else if (reply["Type"] == "Connect" && reply["Status"] == 404)
			{
				std::lock_guard lock(mCommonMutex);
				std::lock_guard connectLock(mConnectMutex);
				mService->Disconnect();
				mHeaderLabel->setText("Room not found");
				mHeaderLabel->setFillColor(sf::Color::Red);
			}
			else
			{
				std::lock_guard lock(mCommonMutex);
				mHeaderLabel->setText("Failed server connection");
				mHeaderLabel->setFillColor(sf::Color::Red);
			}
		}
		catch (json::exception)
		{
			std::lock_guard lock(mCommonMutex);
			mHeaderLabel->setText("Failed server connection");
			mHeaderLabel->setFillColor(sf::Color::Red);
		}
	}
	else
	{
		std::lock_guard lock(mCommonMutex);
		mHeaderLabel->setText("Failed server connection");
		mHeaderLabel->setFillColor(sf::Color::Red);
	}
}

void ConnectionWaitState::backgroundAction(std::future<void> futureObj)
{
	while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
	{
		mReceiveMutex.lock();
		json reply = mService->Receive();
		mReceiveMutex.unlock();

		if (reply["Type"] == "Disconnect")
		{
			std::lock_guard lock(mCommonMutex);
			getContext().gameInfo->getGameDescription().Count = reply["Game"]["Count"];
			std::string header = "Waiting for the player(" + std::to_string(getContext().gameInfo->getGameDescription().Count) + "/2)";
			mHeaderLabel->setFillColor(sf::Color::White);
			mHeaderLabel->setText(header);
		}
		else if (reply["Type"] == "Game" && reply["Status"] == "Start")
		{
			std::lock_guard lock(mCommonMutex);
			mIsGameStarted = true;
			getContext().gameInfo->getGameDescription().Count = reply["Game"]["Count"];
			getContext().gameInfo->setServerService(std::move(mService));
			requestStackClear();
			requestStackPush(States::MultiplayerGame);
		}
	}
}