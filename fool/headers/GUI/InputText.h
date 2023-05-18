#pragma once

#include "Engine/Resource/ResourceHolder.hpp"
#include "Engine/Resource/ResourceIdentifiers.h"
#include "GUI/Component.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <functional>

namespace GUI
{
	class InputText : public Component
	{
	public:
		typedef std::unique_ptr<InputText> Ptr;
		typedef std::function<void()> Callback;
		typedef std::function<bool(sf::Uint32)> InputCallback;

		enum Alignment
		{
			Left,
			Center,
			Right
		};

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
		InputText(FontHolder &fonts, Fonts::ID fontID);

		void setOnEnterAction(Callback callback);
		void setOnInputAction(InputCallback callback);

		virtual bool isSelectable() const;
		virtual void select();
		virtual void deselect();
		virtual void activate();
		virtual void deactivate();

		void setText(const sf::String &text);
		void setAlignment(Alignment alignment);
		void setSize(sf::Vector2f size);
		void setIsAlwaysActive(bool isAlwaysActive);

		void setActiveStyle(Style style);
		void setNotActiveStyle(Style style);

		sf::FloatRect getBounds() const;
		sf::String getString();

		virtual void handleEvent(const sf::Event &event);
		virtual Component::Type getType() const;

	private:
		void applyStyle(Style style);
		void setOrigin();

		virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		Style mActiveStyle;
		Style mNotActiveStyle;

		bool mIsAlwaysActive;

		Callback mOnEnter;
		InputCallback mOnInput;

		sf::RectangleShape mShape;
		sf::Text mText;
		Alignment mAlignment;
	};
}
