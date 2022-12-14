#include "PauseState.h"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.h"
#include "Utility.h"

#include <SFML/Graphics/RenderWindow.hpp>

PauseState::PauseState(StateStack &stack, Context context)
	: State(stack, context)
{
	mMenuSprite.setTexture(context.textures->get(Textures::ID(Textures::Global::PauseMenu)));
	centerOrigin(mMenuSprite);
}

void PauseState::draw()
{
	sf::RenderWindow &window = *getContext().window;
	window.draw(mMenuSprite);
}

bool PauseState::update(sf::Time dt)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event &event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			requestStackPop();
			break;
		}
	}
	return false;
}
