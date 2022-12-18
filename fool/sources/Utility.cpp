#include "Utility.h"

#include <cmath>
#include <assert.h>
#include <fstream>
#include <string>

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

std::string getFilePath(Textures::Global::ID global, Textures::Local::ID local)
{
	if (local == Textures::Local::None)
	{
		return getFilePath(global);
	}
	else if (local == Textures::Local::Main)
	{
		std::string result = "resources/img/";
		switch (global)
		{
		case Textures::Global::Diamonds:
			result += "Diamonds";
			break;
		case Textures::Global::Hearts:
			result += "Hearts";
			break;
		case Textures::Global::Clubs:
			result += "Clubs";
			break;
		case Textures::Global::Spades:
			result += "Spades";
			break;
		default:
			assert(false);
		}
		result += "/Main.png";
		return result;
	}
	else
	{
		// convert global & local to suit & rank
		if (global == Textures::Global::Diamonds ||
			global == Textures::Global::Hearts ||
			global == Textures::Global::Clubs ||
			global == Textures::Global::Spades)
		{
			getFilePath(Card::Suit(global), Card::Rank(local));
		}
		else
		{
			assert(false);
		}
	}
}

std::string getFilePath(Textures::Global::ID global)
{
	if (global == Textures::Global::None ||
		global == Textures::Global::Diamonds ||
		global == Textures::Global::Hearts ||
		global == Textures::Global::Clubs ||
		global == Textures::Global::Spades)
	{
		assert(false);
	}

	switch (global)
	{
	case Textures::Global::Background:
		return "resources/img/background.jpg";
	case Textures::Global::MainMenu:
		return "resources/img/mainMenu.png";
	case Textures::Global::PauseMenu:
		return "resources/img/pauseMenu.png";
	case Textures::Global::ReverseFace:
		return "resources/img/reverse_face.png";
	default:
		assert(false);
	}
}

std::string getFilePath(Textures::ID texturesID)
{
	return getFilePath(texturesID.getGlobal(), texturesID.getLocal());
}

std::string getFilePath(Fonts::ID fontID)
{
	switch (fontID)
	{
	case Fonts::Main:
		return "resources/fonts/Sansation.ttf";
	case Fonts::Label:
		return "resources/fonts/Gentlemens Script.otf";
	default:
		assert(false);
	}
	return "";
}

void centerOrigin(sf::Sprite &sprite)
{
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
}

void centerOrigin(sf::Text &text)
{
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(std::floor(bounds.width / 2.f), std::floor((bounds.height + bounds.top) * 1.45f / 2.f));
}

void centerOrigin(sf::RectangleShape &shape)
{
	sf::FloatRect bounds = shape.getLocalBounds();
	shape.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
}

std::map<std::string, int> getStatistics()
{
	std::map<std::string, int> result;
	std::ifstream fin(STATISTICPATH);
	if (!fin.is_open())
	{
		std::ofstream fout(STATISTICPATH);
		fout << "total: 0\nvictory: 0\nlose: 0\ndraw: 0";
		fout.close();
		fin.close();
		fin.open(STATISTICPATH);
	}
	std::string str;

	std::getline(fin, str);
	result["total"] = std::stoi(str.erase(0, 7)); // erase "total: "

	std::getline(fin, str);
	result["victory"] = std::stoi(str.erase(0, 9)); // erase "victory: "

	std::getline(fin, str);
	result["lose"] = std::stoi(str.erase(0, 6)); // erase "lose: "

	std::getline(fin, str);
	result["draw"] = std::stoi(str.erase(0, 6)); // erase "draw: "

	fin.close();

	return result;
}

void setStatistics(std::map<std::string, int> statistics)
{
	std::ofstream fout(STATISTICPATH);
	fout << "total: " << statistics["total"] << "\nvictory: " << statistics["victory"] << "\nlose: " << statistics["lose"] << "\ndraw: " << statistics["draw"];
	fout.close();
}