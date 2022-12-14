#pragma once

#include "State.h"

class StatisticState : public State
{
public:
	StatisticState(StateStack &stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event &event);
};