#include "Game/PlayerInfo.h"

PlayerInfo::PlayerInfo() : Name("")
{
}

void PlayerInfo::setData(const PlayerInfo &playerInfo)
{
	Name = playerInfo.getName();
}

void PlayerInfo::setName(const sf::String &name)
{
	Name = name;
}

sf::String PlayerInfo::getName() const
{
	return Name != "" ? Name : "Anonymous";
}
