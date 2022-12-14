#pragma once

#include <SFML/System/NonCopyable.hpp>

class GameStatus : sf::NonCopyable
{
public:
	enum Status
	{
		None,
		InGame,
		PlayerWon,
		ComputerWon,
		Draw
	};

public:
	GameStatus();

	Status getCurrentStatus();
	void setCurrentStatus(Status status);

private:
	Status currentStatus;
};