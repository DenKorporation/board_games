#pragma once

#include "Card.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <map>
#include <string>

const std::string STATISTICPATH = "statistic.txt";

std::wstring utf8_to_wstring(const std::string &str);
std::string wstring_to_utf8(const std::wstring &str);

std::string getFilePath(Card::Suit suit, Card::Rank rank);
std::string getFilePath(Textures::Global::ID global, Textures::Local::ID local);
std::string getFilePath(Textures::Global::ID global);
std::string getFilePath(Textures::ID texturesID);

std::string getFilePath(Fonts::ID fontID);

std::string getFilePath(Sounds::ID soundID);
std::string getFilePath(Musics::ID musicID);

void centerOrigin(sf::Sprite &sprite);
void centerOrigin(sf::Text &text);
void centerOrigin(sf::RectangleShape &shape);

std::map<std::string, int> getStatistics();
void setStatistics(std::map<std::string, int> statistics);