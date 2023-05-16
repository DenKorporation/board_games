#pragma once

#include <SFML/System/String.hpp>

#include <string>

struct GameDescription
{
	std::string Id;
	sf::String Name;
	int Count;

	GameDescription() : Id(""), Name(""), Count(0)
	{
	}
};
