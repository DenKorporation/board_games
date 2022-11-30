#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>

class Application : private sf::NonCopyable
{
public:
	Application();
	void run();

private:
	void update(sf::Time dt);
	void handleInput();
	void render();

	void updateStatisticText();

private:
	static const sf::Time mFrameTime;

	sf::Time mTimeSinceLastUpdate;
	int mNumFrame;
	sf::RenderWindow mWindow;
};