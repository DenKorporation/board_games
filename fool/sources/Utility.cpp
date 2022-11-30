#include "Utility.h"

// return filepath to card texture
std::string getFilePath(Card::Suit suit, Card::Rank rank)
{
	std::string result = "resources/img/";

	std::string suitStr = "";
	std::string rankStr = "";

	switch (suit)
	{
	case Card::Diamonds:
		suitStr = "Diamonds";
		break;
	case Card::Hearts:
		suitStr = "Hearts";
		break;
	case Card::Clubs:
		suitStr = "Clubs";
		break;
	case Card::Spades:
		suitStr = "Spades";
		break;
	}

	if (rank >= Card::_2 && rank <= Card::_10)
	{
		rankStr = std::to_string(int(rank - Card::_2) + 2);
	}
	else
	{
		switch (rank)
		{
		case Card::_Jack:
			rankStr = "Jack";
			break;
		case Card::_Queen:
			rankStr = "Queen";
			break;
		case Card::_King:
			rankStr = "King";
			break;
		case Card::_Ace:
			rankStr = "Ace";
			break;
		}
	}

	result += suitStr + "/" + rankStr + ".png";

	return result;
}