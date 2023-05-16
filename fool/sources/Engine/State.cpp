#include "Engine/State.h"
#include "Engine/StateStack.h"

State::Context::Context(sf::RenderWindow &window, TextureHolder &textures, FontHolder &fonts, SoundHolder &sounds, GameInfo &gameInfo)
	: window(&window),
	  textures(&textures),
	  fonts(&fonts),
	  sounds(&sounds),
	  gameInfo(&gameInfo)
{
}

State::State(StateStack &stack, Context context)
	: mStack(&stack),
	  mContext(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStackClear()
{
	mStack->clearStack();
}

State::Context State::getContext() const
{
	return mContext;
}
