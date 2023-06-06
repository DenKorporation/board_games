#include "Game/GameInfo.h"

GameInfo::GameInfo()
	: currentStatus(Status::None), curGame()
{
}

GameInfo::Status GameInfo::getCurrentStatus()
{
	return currentStatus;
}

void GameInfo::setCurrentStatus(Status status)
{
	currentStatus = status;
}

void GameInfo::setGameDescription(const GameDescription &gameDescription)
{
	curGame = gameDescription;
}

GameDescription &GameInfo::getGameDescription()
{
	return curGame;
}

void GameInfo::setServerService(std::unique_ptr<ServerService> service)
{
	curService = std::move(service);
}

std::unique_ptr<ServerService> GameInfo::getServerService()
{
	return std::move(curService);
}
