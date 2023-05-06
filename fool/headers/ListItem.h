#pragma once

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.h"
#include "Component.h"
#include "Offset.h"

#include <SFML/Graphics/Text.hpp>
#include <any>

namespace GUI
{
	class ListItem : public Component
	{
	public:
		typedef std::unique_ptr<ListItem> Ptr;

	public:
		ListItem(Offset margin);

		virtual void setSize(sf::Vector2f size) = 0;

		virtual bool isSelectable() const;
		virtual void handleEvent(const sf::Event &event);
		virtual Component::Type getType() const;

		virtual std::any getData() const = 0;
		sf::Transform getWorldTransform() const;
		void setScrollTranslation(sf::Vector2f translation);

		virtual sf::FloatRect getMinBounds() const = 0;
		virtual sf::FloatRect getFullBounds() const = 0;

		virtual void applyNormalStyle() = 0;
		virtual void applyHoverStyle() = 0;
		virtual void applySelectedStyle() = 0;

	private:
		virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const = 0;

	protected:
		Offset margin;
		sf::Vector2f mScrollTranslation;
	};
}
