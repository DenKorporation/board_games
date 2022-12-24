#pragma once

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.h"
#include "StateStack.h"
#include "GameStatus.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Text.hpp>

class Application : private sf::NonCopyable
{
public:
	Application();
	void run();

private:
	void update(sf::Time dt);
	void processInput();
	void render();

	void updateStatisticText(sf::Time dt);
	void registerStates();

private:
	static const sf::Time TimePerFrame;

	sf::RenderWindow mWindow;
	TextureHolder mTextures;
	FontHolder mFonts;
	SoundHolder mSounds;
	GameStatus mGameStatus;

	StateStack mStateStack;

	sf::Text mStatisticsText;
	sf::Time mStatisticsUpdateTime;
	int mStatisticsNumFrames;
};