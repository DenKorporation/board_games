#include "GUI/Label.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace GUI
{
	Label::Label(const std::string &text, FontHolder &fonts, Fonts::ID fontID)
		: mText(text, fonts.get(fontID), 16)
	{
		centerOrigin(mText);
	}

	bool Label::isSelectable() const
	{
		return false;
	}

	void Label::handleEvent(const sf::Event &event)
	{
	}

	Component::Type Label::getType() const
	{
		return Component::Label;
	}

	void Label::setText(const std::string &text)
	{
		mText.setString(text);
		centerOrigin(mText);
	}

	void Label::setFontSize(unsigned int size)
	{
		mText.setCharacterSize(size);
		centerOrigin(mText);
	}

	void Label::setOutlineThickness(float thickness)
	{
		mText.setOutlineThickness(thickness);
		centerOrigin(mText);
	}

	void Label::setLetterSpacing(float letterSpacing)
	{
		mText.setLetterSpacing(letterSpacing);
		centerOrigin(mText);
	}

	void Label::setFillColor(sf::Color color)
	{
		mText.setFillColor(color);
	}

	sf::FloatRect Label::getBounds() const
	{
		return getWorldTransform().transformRect(mText.getGlobalBounds());
	}

	void Label::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
	{
		target.draw(mText, states);
	}
}