#pragma once

#include "Component.h"

#include <vector>
#include <memory>

namespace GUI
{
	class Container : public Component
	{
	public:
		typedef std::unique_ptr<Container> Ptr;

	public:
		Container();

		virtual bool isSelectable() const;
		virtual void handleEvent(const sf::Event &event);

		void attachChild(Component::Ptr child);
		Component::Ptr detachChild(const Component &node);

		bool hasSelection() const;
		virtual Component::Type getType() const;

	private:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
		virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
		virtual void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		std::vector<Component::Ptr> mChildren;
		int mSelectedChild;
	};
}