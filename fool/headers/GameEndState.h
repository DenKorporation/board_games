#pragma once

#include "State.h"
#include "Container.h"

#include <SFML/Graphics/Sprite.hpp>

class GameEndState : public State
{
public:
	GameEndState(StateStack &stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event &event);

private:
	SceneNode mSceneGraph;
	GUI::Container *mGUIContainer;
};