#include "MenuState.h"
MenuState::MenuState(StateStack &stack, Context context)
	: State(stack, context)
{
}

void MenuState::draw()
{
}

bool MenuState::update(sf::Time dt)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event &event)
{
	return true;
}
