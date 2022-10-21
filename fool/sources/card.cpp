#include "card.h"

Card::Card(card_rank _rank, std::string texture_path) : rank(_rank)
{
	if (!texture.loadFromFile(texture_path))
	{
	}
	else
	{
		// throw
	}
}

card_rank Card::get_rank()
{
	return this->rank;
}