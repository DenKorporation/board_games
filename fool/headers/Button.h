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

		struct Style
		{
			Style(unsigned int fontSize, sf::Color textColor, sf::Color fillColor,
				  sf::Color outlineColor, float outlineThickness);
			unsigned int fontSize;
			sf::Color textColor;
			sf::Color fillColor;
			sf::Color outlineColor;
			float outlineThickness;
		};

	public:
		Button(const FontHolder &fonts);

		void setCallback(Callback callback);
		void setText(const std::string &text);
		void setSize(sf::Vector2f size);

		virtual bool isSelectable() const;
		virtual void select();
		virtual void deselect();
		virtual void activate();

		void setNormalStyle(Style style);
		void setSelectedStyle(Style style);

		virtual void handleEvent(const sf::Event &event);
		virtual Component::Type getType() const;

	private:
		void applyStyle(Style style);

		virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		Style mNormalStyle;
		Style mSelectedStyle;

		Callback mCallback;
		sf::RectangleShape mShape;
		sf::Text mText;
	};
}
