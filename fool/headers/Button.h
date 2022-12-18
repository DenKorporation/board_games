#pragma once

#include "Component.h"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <memory>
#include <vector>
#include <string>
#include <functional>

namespace GUI
{
	class Button : public Component
	{
	public:
		typedef std::unique_ptr<Button> Ptr;
		typedef std::function<void()> Callback;

	public:
		Button(const FontHolder &fonts);

		void setCallback(Callback callback);
		void setText(const std::string &text);
		void setSize(sf::Vector2f size);
		void setFontSize(unsigned int size);

		virtual bool isSelectable() const;
		virtual void select();
		virtual void deselect();
		virtual void activate();

		virtual void handleEvent(const sf::Event &event);
		virtual Component::Type getType() const;

	private:
		virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		Callback mCallback;
		sf::RectangleShape mShape;
		sf::Text mText;
	};
}
