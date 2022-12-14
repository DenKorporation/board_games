#pragma once

#include "State.h"
#include "GameStatus.h"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.h"
#include "Card.h"

#include <vector>

class GameState : public State
{
public:
	GameState(StateStack &stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event &event);

private:
	TextureHolder mTextures;

	sf::Time time;

	std::vector<std::vector<Card>> mCards;
};