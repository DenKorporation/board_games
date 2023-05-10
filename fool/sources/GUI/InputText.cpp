#include "GUI/InputText.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace GUI
{
	InputText::InputText(FontHolder &fonts, Fonts::ID fontID)
		: mText("", fonts.get(fontID), 16)
	{
	}

	bool InputText::isSelectable() const
	{
		return true;
	}

	void InputText::handleEvent(const sf::Event &event)
	{
		if (event.type == sf::Event::TextEntered)
		{
			sf::String text = mText.getString();
			if (event.text.unicode == 8)
			{
				if (!text.isEmpty())
				{
					text.erase(text.getSize() - 1, 1);
				}
			}
			else if (event.text.unicode == 13)
			{
			}
			else
			{
				text.insert(text.getSize(), sf::String(event.text.unicode));
			}
			mText.setString(text);
			setOrigin();
		}
	}

	Component::Type InputText::getType() const
	{
		return Component::InputText;
	}

	void InputText::setText(const std::string &text)
	{
		mText.setString(text);
		setOrigin();
	}

	void InputText::setFontSize(unsigned int size)
	{
		mText.setCharacterSize(size);
		setOrigin();
	}

	void InputText::setOutlineThickness(float thickness)
	{
		mText.setOutlineThickness(thickness);
		setOrigin();
	}

	void InputText::setLetterSpacing(float letterSpacing)
	{
		mText.setLetterSpacing(letterSpacing);
		setOrigin();
	}

	void InputText::setFillColor(sf::Color color)
	{
		mText.setFillColor(color);
	}

	void InputText::setAlignment(Alignment alignment)
	{
		mAlignment = alignment;
		setOrigin();
	}

	sf::FloatRect InputText::getBounds() const
	{
		return getWorldTransform().transformRect(mText.getGlobalBounds());
	}

	sf::String InputText::getString()
	{
		return mText.getString();
	}

	void InputText::setOrigin()
	{
		centerOrigin(mText);
		switch (mAlignment)
		{
		case Left:
			mText.setOrigin(0.f, mText.getOrigin().y);
			break;
		case Right:
			mText.setOrigin(mText.getLocalBounds().width, mText.getOrigin().y);
			break;
		}
	}

	void InputText::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
	{
		target.draw(mText, states);
	}
}