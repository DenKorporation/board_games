#pragma once

#include "Engine/State.h"
#include "GUI/Container.h"
#include "GUI/InputText.h"

#include <SFML/Graphics/Sprite.hpp>

class CreatingRoomState : public State
{
public:
	CreatingRoomState(StateStack &stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event &event);

private:
	bool createRoom(sf::String text);

private:
	SceneNode mSceneGraph;
	GUI::Container *mGUIContainer;
	GUI::InputText *mInputText;
};