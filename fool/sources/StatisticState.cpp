#include "StatisticState.h"

StatisticState::StatisticState(StateStack &stack, Context context)
	: State(stack, context)
{
}

void StatisticState::draw()
{
}

bool StatisticState::update(sf::Time dt)
{
	return true;
}

bool StatisticState::handleEvent(const sf::Event &event)
{
	return true;
}
