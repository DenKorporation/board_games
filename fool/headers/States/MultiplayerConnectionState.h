#pragma once

#include "Engine/State.h"
#include "GUI/Container.h"
#include "GUI/List.h"

#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>

class MultiplayerConnectionState : public State
{
public:
	MultiplayerConnectionState(StateStack &stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event &event);

private:
	void initConnectionList();
	~MultiplayerConnectionState();

private:
	SceneNode mSceneGraph;
	GUI::Container *mGUIContainer;
	GUI::List *mConnectionList;

	sf::Time timeSinceLastUpdate;

	sf::Mutex mMutex;
	sf::Thread mInitListThread;
	bool mIsThreadRunning;
};