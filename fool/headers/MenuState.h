#pragma once

#include "State.h"

class MenuState : public State
{
public:
	MenuState(StateStack &stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event &event);
};