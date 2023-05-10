#pragma once

#include "Engine/Resource/ResourceHolder.hpp"
#include "Engine/Resource/ResourceIdentifiers.h"
#include "GUI/Component.h"

#include <SFML/Graphics/Text.hpp>

namespace GUI
{
	class InputText : public Component
	{
	public:
		typedef std::unique_ptr<InputText> Ptr;

		enum Alignment
		{
			Left,
			Center,
			Right
		};

	public:
		InputText(FontHolder &fonts, Fonts::ID fontID);

		virtual bool isSelectable() const;
		virtual void handleEvent(const sf::Event &event);
		virtual Component::Type getType() const;

		void setText(const std::string &text);
		void setFontSize(unsigned int size);
		void setOutlineThickness(float thickness);
		void setLetterSpacing(float letterSpacing);
		void setFillColor(sf::Color color);
		void setAlignment(Alignment alignment);

		sf::FloatRect getBounds() const;
		sf::String getString();

	private:
		void setOrigin();
		virtual void
		drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		sf::Text mText;
		Alignment mAlignment;
	};
}
