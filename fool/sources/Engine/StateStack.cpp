#include "Engine/StateStack.h"

#include <cassert>

StateStack::StateStack(State::Context context)
	: mStack(),
	  mPendingList(),
	  mContext(context),
	  mFactories()
{
}

void StateStack::draw()
{
	for (State::Ptr &state : mStack)
	{
		state->draw();
	}
}

void StateStack::update(sf::Time dt)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
		{
			break;
		}
	}
	applyPendingChange();
}

void StateStack::handleEvent(const sf::Event &event)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(event))
		{
			break;
		}
	}
	applyPendingChange();
}

void StateStack::pushState(States::ID stateID)
{
	mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStack()
{
	mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
	return mStack.empty();
}

State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());

	return found->second();
}

void StateStack::applyPendingChange()
{
	for (PendingChange change : mPendingList)
	{
		switch (change.action)
		{
		case Push:
			mStack.push_back(createState(change.stateID));
			break;
		case Pop:
			mStack.pop_back();
			break;
		case Clear:
			mStack.clear();
			break;
		}
	}
	mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
	: action(action),
	  stateID(stateID)
{
}
