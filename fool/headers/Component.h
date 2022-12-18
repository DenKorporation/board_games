#pragma once

#include "SceneNode.h"

#include <SFML/Window/Event.hpp>

#include <memory>

namespace GUI
{
	class Component : public SceneNode
	{
	public:
		enum Type
		{
			Container,
			Label,
			Button
		};
		typedef std::unique_ptr<Component> Ptr;

	public:
		Component();
		virtual ~Component();
		virtual bool isSelectable() const = 0;
		virtual bool isSelected() const;
		virtual void select();
		virtual void deselect();
		virtual bool isActive() const;
		virtual void activate();
		virtual void deactivate();

		virtual Type getType() const = 0;

		virtual void attachChild(SceneNode::Ptr child);
		virtual SceneNode::Ptr detachChild(const SceneNode &node);

		virtual void handleEvent(const sf::Event &event) = 0;

	private:
		bool mIsSelected;
		bool mIsActive;
	};
}