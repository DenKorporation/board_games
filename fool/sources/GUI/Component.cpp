#include "GUI/Component.h"

#include <cassert>

namespace GUI
{
	Component::Component() : mIsSelected(false), mIsActive(false)
	{
	}

	Component::~Component()
	{
	}

	bool Component::isSelected() const
	{
		return mIsSelected;
	}

	void Component::select()
	{
		mIsSelected = true;
	}

	void Component::deselect()
	{
		mIsSelected = false;
	}

	bool Component::isActive() const
	{
		return mIsActive;
	}

	void Component::activate()
	{
		mIsActive = true;
	}

	void Component::deactivate()
	{
		mIsActive = false;
	}

	void Component::attachChild(SceneNode::Ptr child)
	{
		assert(false);
	}

	SceneNode::Ptr Component::detachChild(const SceneNode &node)
	{
		assert(false);
		return nullptr;
	}
}