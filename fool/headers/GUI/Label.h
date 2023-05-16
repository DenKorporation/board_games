#pragma once

#include "Engine/Resource/ResourceHolder.hpp"
#include "Engine/Resource/ResourceIdentifiers.h"
#include "GUI/Component.h"

#include <SFML/Graphics/Text.hpp>

namespace GUI
{
	class Label : public Component
	{
	public:
		typedef std::unique_ptr<Label> Ptr;

	public:
		Label(const sf::String &text, FontHolder &fonts, Fonts::ID fontID);

		virtual bool isSelectable() const;
		virtual void handleEvent(const sf::Event &event);
		virtual Component::Type getType() const;

		void setText(const std::string &text);
		void setUtf8Text(const sf::String &text);
		void setFontSize(unsigned int size);
		void setOutlineThickness(float thickness);
		void setLetterSpacing(float letterSpacing);
		void setFillColor(sf::Color color);

		sf::FloatRect getBounds() const;

	private:
		virtual void
		drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		sf::Text mText;
	};
}
