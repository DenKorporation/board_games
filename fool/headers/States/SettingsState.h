#pragma once

#include "Engine/State.h"
#include "GUI/Container.h"
#include "GUI/Label.h"
#include "GUI/InputText.h"

#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>

class SettingsState : public State
{
public:
	SettingsState(StateStack &stack, Context context);
	~SettingsState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event &event);

private:
	void checkConnection();

private:
	SceneNode mSceneGraph;
	GUI::Container *mGUIContainer;

	GUI::Label *mReplyLabel;
	GUI::InputText *mNameInput;
	GUI::InputText *mIpInput;
	GUI::InputText *mPortInput;

	sf::Thread mConnectionThread;
	sf::Mutex mMutex;
	bool mIsThreadRunning;
};
