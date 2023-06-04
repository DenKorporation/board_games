#pragma once

#include <SFML/System/NonCopyable.hpp>

#include "GameDescription.h"
#include "ServerService.h"

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

	void setServerService(std::unique_ptr<ServerService> service);
	ServerService &getServerService();

private:
	Status currentStatus;
	GameDescription curGame;
	std::unique_ptr<ServerService> curService;
};