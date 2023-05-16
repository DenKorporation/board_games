#pragma once

#include <SFML/System/NonCopyable.hpp>

#include "GameDescription.h"

class GameInfo : sf::NonCopyable
{
public:
	enum Status
	{
		None,
		InGame,
		PlayerWon,
		EnemyWon,
		Draw
	};

public:
	GameInfo();

	Status getCurrentStatus();
	void setCurrentStatus(Status status);

	void setGameDescription(const GameDescription &gameDescription);
	GameDescription &getGameDescription();

private:
	Status currentStatus;
	GameDescription curGame;
};