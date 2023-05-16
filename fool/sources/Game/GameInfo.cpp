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