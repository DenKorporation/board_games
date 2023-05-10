#include "Game/GameStatus.h"

GameStatus::GameStatus()
	: currentStatus(Status::None)
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