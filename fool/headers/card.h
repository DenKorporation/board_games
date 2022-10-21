#pragma once

#include <SFML/Graphics.hpp>
#include <string>

enum card_rank
{
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,
	_10,
	_J,
	_Q,
	_K,
	_A
};


class Card
{
private:
	card_rank rank;
	sf::Texture texture;
public:
	Card(card_rank _rank, std::string texture_path);

	card_rank get_rank();
};