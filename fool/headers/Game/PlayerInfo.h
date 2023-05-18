#pragma once

#include <SFML/System/String.hpp>

class PlayerInfo
{
public:
	PlayerInfo();

	void setData(const PlayerInfo &playerInfo);
	void setName(const sf::String &name);
	sf::String getName() const;

private:
	sf::String Name;
};