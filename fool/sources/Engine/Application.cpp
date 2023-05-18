#include "Engine/Application.h"
#include "Engine/State.h"
#include "States/StateIdentifiers.h"
#include "Utility.h"
#include "States/MenuState.h"
#include "States/GameState.h"
#include "States/PauseState.h"
#include "States/StatisticState.h"
#include "States/GameEndState.h"
#include "States/MultiplayerConnectionState.h"
#include "States/CreatingRoomState.h"
#include "States/ConnectionWaitState.h"

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 144.f);

Application::Application()
	: mWindow(sf::VideoMode(1920, 1080), "Fool", sf::Style::Fullscreen),
	  // : mWindow(sf::VideoMode(1600, 900), "Fool"),
	  // : mWindow(sf::VideoMode(900, 600), "Fool"),
	  mTextures(),
	  mFonts(),
	  mSounds(),
	  mGameInfo(),
	  mPlayerInfo(),
	  mStateStack(State::Context(mWindow, mTextures, mFonts, mSounds, mGameInfo, mPlayerInfo)),
	  mStatisticsText(),
	  mStatisticsUpdateTime(),
	  mStatisticsNumFrames(0)
{
	mPlayerInfo.setData(loadPlayerInfo());

	// mWindow.setKeyRepeatEnabled(false);
	mFonts.load(Fonts::Main, getFilePath(Fonts::Main));
	mFonts.load(Fonts::Label, getFilePath(Fonts::Label));
	mFonts.load(Fonts::Russian, getFilePath(Fonts::Russian));

	mSounds.load(Sounds::CardSelect, getFilePath(Sounds::CardSelect));
	mSounds.load(Sounds::Error, getFilePath(Sounds::Error));
	mSounds.load(Sounds::ButtonSelect, getFilePath((Sounds::ButtonSelect)));
	mSounds.load(Sounds::Shuffle, getFilePath(Sounds::Shuffle));
	mSounds.load(Sounds::CardFlip, getFilePath(Sounds::CardFlip));

	mTextures.load(Textures::Global::Background, getFilePath(Textures::Global::Background));

	// later implement button and label
	mTextures.load(Textures::Global::MainMenu, getFilePath(Textures::Global::MainMenu));
	mTextures.load(Textures::Global::PauseMenu, getFilePath(Textures::Global::PauseMenu));

	sf::Vector2f currentWindowSize = sf::Vector2f(mWindow.getSize().x, mWindow.getSize().y);
	sf::Vector2f defaultWindowSize = sf::Vector2f(1920.f, 1080.f);
	sf::Vector2f windowScale = sf::Vector2f(currentWindowSize.x / defaultWindowSize.x, currentWindowSize.y / defaultWindowSize.y);
	mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setCharacterSize(20u * windowScale.y);
	mStatisticsText.setOutlineThickness(2.f * windowScale.y);
	mStatisticsText.setPosition(5.f * windowScale.x, 5.f * windowScale.y);

	registerStates();

	mStateStack.pushState(States::Menu);
}

Application::~Application()
{
	savePlayerInfo(mPlayerInfo);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate >= TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			if (mStateStack.isEmpty())
			{
				mWindow.close();
			}
		}

		updateStatisticText(dt);
		render();
	}
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);
		if (event.type == sf::Event::Closed)
		{
			mWindow.close();
		}
	}
}

void Application::render()
{
	mWindow.clear();
	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);

	mWindow.display();
}

void Application::updateStatisticText(sf::Time dt)
{
	mStatisticsNumFrames++;
	mStatisticsUpdateTime += dt;

	if (mStatisticsUpdateTime >= sf::seconds(1.f))
	{
		mStatisticsText.setString("FPS: " + std::to_string(mStatisticsNumFrames) + "\n" +
								  "Update: " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "ms");
		mStatisticsNumFrames = 0;
		mStatisticsUpdateTime -= sf::seconds(1.f);
	}
}

void Application::registerStates()
{
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<PauseState>(States::Pause);
	mStateStack.registerState<StatisticState>(States::Statistic);
	mStateStack.registerState<GameEndState>(States::GameEnd);
	mStateStack.registerState<MultiplayerConnectionState>(States::MultiplayerConnection);
	mStateStack.registerState<CreatingRoomState>(States::CreatingRoom);
	mStateStack.registerState<ConnectionWaitState>(States::ConnectionWait);
}