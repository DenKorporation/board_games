#include "Application.h"
#include "State.h"
#include "StateIdentifiers.h"
#include "Utility.h"
#include "MenuState.h"
#include "GameState.h"
#include "PauseState.h"
#include "StatisticState.h"

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 144.f);

Application::Application()
	: mWindow(sf::VideoMode(1920, 1080), "Fool", sf::Style::Fullscreen),
	  // : mWindow(sf::VideoMode(900, 600), "Fool"),
	  mTextures(),
	  mFonts(),
	  mGameStatus(),
	  mStateStack(State::Context(mWindow, mTextures, mFonts, mGameStatus)),
	  mStatisticsText(),
	  mStatisticsUpdateTime(),
	  mStatisticsNumFrames(0)
{

	mWindow.setKeyRepeatEnabled(false);
	mFonts.load(Fonts::Main, getFilePath(Fonts::Main));
	mFonts.load(Fonts::Label, getFilePath(Fonts::Label));

	mTextures.load(Textures::Global::Background, getFilePath(Textures::Global::Background));

	// later implement button and label
	mTextures.load(Textures::Global::MainMenu, getFilePath(Textures::Global::MainMenu));
	mTextures.load(Textures::Global::PauseMenu, getFilePath(Textures::Global::PauseMenu));

	mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setOutlineThickness(2.f);
	mStatisticsText.setCharacterSize(20u);
	mStatisticsText.setPosition(5.f, 5.f);

	registerStates();

	// change to States::Menu
	mStateStack.pushState(States::Menu);
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
}