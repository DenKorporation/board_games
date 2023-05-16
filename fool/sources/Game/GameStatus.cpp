#include "Game/GameStatus.h"

GameStatus::GameStatus()
	: currentStatus(Status::None), curGame()
{
}

GameStatus::Status GameStatus::getCurrentStatus()
{
	return currentStatus;
}

void GameStatus::setCurrentStatus(Status status)
{
	currentStatus = status;
}

void GameStatus::setGameDescription(const GameDescription &gameDescription)
{
	curGame = gameDescription;
}

GameDescription &GameStatus::getGameDescription()
{
	return curGame;
}