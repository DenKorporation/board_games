#pragma once

#include "Engine/State.h"
#include "GUI/Container.h"

#include <SFML/Graphics/Sprite.hpp>

class MenuState : public State
{
public:
	MenuState(StateStack &stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event &event);

private:
	SceneNode mSceneGraph;
	GUI::Container *mGUIContainer;
};