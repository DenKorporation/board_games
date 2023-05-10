#pragma once

#include "Engine/State.h"
#include "GUI/Container.h"
#include "GUI/List.h"

class MultiplayerConnectionState : public State
{
public:
	MultiplayerConnectionState(StateStack &stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event &event);

private:
	void initConnectionList();

private:
	SceneNode mSceneGraph;
	GUI::Container *mGUIContainer;
	GUI::List *mConnectionList;
};