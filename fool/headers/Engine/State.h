#pragma once

#include "States/StateIdentifiers.h"
#include "Engine/Resource/ResourceIdentifiers.h"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

namespace sf
{
	class RenderWindow;
}

class StateStack;
class GameInfo;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	struct Context
	{
		Context(sf::RenderWindow &window, TextureHolder &textures, FontHolder &fonts, SoundHolder &sounds, GameInfo &gameInfo);

		sf::RenderWindow *window;
		TextureHolder *textures;
		FontHolder *fonts;
		SoundHolder *sounds;
		GameInfo *gameInfo;
	};

public:
	State(StateStack &stack, Context context);
	virtual ~State();

	virtual void draw() = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(const sf::Event &event) = 0;

protected:
	void requestStackPush(States::ID stateID);
	void requestStackPop();
	void requestStackClear();

	Context getContext() const;

private:
	StateStack *mStack;
	Context mContext;
};