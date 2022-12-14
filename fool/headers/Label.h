#pragma once

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.h"
#include "Component.h"

#include <SFML/Graphics/Text.hpp>

namespace GUI
{
	class Label : public Component
	{
	public:
		typedef std::unique_ptr<Label> Ptr;

	public:
		Label(const std::string &text, FontHolder &fonts, Fonts::ID fontID);

		virtual bool isSelectable() const;
		virtual void handleEvent(const sf::Event &event);
		virtual Component::Type getType() const;

		void setText(const std::string &text);
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
