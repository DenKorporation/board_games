#pragma once

#include "Card.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <string>

std::string getFilePath(Card::Suit suit, Card::Rank rank);

std::string getFilePath(Textures::Global::ID global, Textures::Local::ID local);

std::string getFilePath(Textures::Global::ID global);

std::string getFilePath(Textures::ID texturesID);

std::string getFilePath(Fonts::ID fontID);

void centerOrigin(sf::Sprite &sprite);
void centerOrigin(sf::Text &text);
void centerOrigin(sf::RectangleShape &shape);