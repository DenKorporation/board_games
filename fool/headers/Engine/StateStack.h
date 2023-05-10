#pragma once

#include "Engine/State.h"
#include "States/StateIdentifiers.h"
#include "Engine/Resource/ResourceIdentifiers.h"

#include <SFML/System/NonCopyable.hpp>

#include <vector>
#include <functional>
#include <map>

class StateStack : sf::NonCopyable
{
public:
	enum Action
	{
		Push,
		Pop,
		Clear
	};

public:
	explicit StateStack(State::Context context);

	template <typename T>
	void registerState(States::ID stateID);

	void draw();
	void update(sf::Time dt);
	void handleEvent(const sf::Event &event);

	void pushState(States::ID stateID);
	void popState();
	void clearStack();

	bool isEmpty() const;

private:
	State::Ptr createState(States::ID stateID);
	void applyPendingChange();

private:
	struct PendingChange
	{
		explicit PendingChange(Action action, States::ID stateID = States::None);

		Action action;
		States::ID stateID;
	};

private:
	std::vector<State::Ptr> mStack;
	std::vector<PendingChange> mPendingList;

	State::Context mContext;
	std::map<States::ID, std::function<State::Ptr()>> mFactories;
};

template <typename T>
void StateStack::registerState(States::ID stateID)
{
	mFactories[stateID] = [this]()
	{
		return State::Ptr(new T(*this, mContext));
	};
}