#pragma once

#include "Engine/Resource/ResourceHolder.hpp"
#include "Engine/Resource/ResourceIdentifiers.h"
#include "Engine/StateStack.h"
#include "Game/GameInfo.h"
#include "Game/PlayerInfo.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Text.hpp>

class Application : private sf::NonCopyable
{
public:
	Application();
	~Application();
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
	GameInfo mGameInfo;
	PlayerInfo mPlayerInfo;

	StateStack mStateStack;

	sf::Text mStatisticsText;
	sf::Time mStatisticsUpdateTime;
	int mStatisticsNumFrames;
};