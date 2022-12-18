#include "Button.h"
#include "Utility.h"

#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

#include <iostream>

namespace GUI
{
	Button::Button(const FontHolder &fonts)
		: mText("", fonts.get(Fonts::Main), 30),
		  mCallback(),
		  mShape()
	{
		mText.setFillColor(sf::Color(255, 255, 255));
		mShape.setFillColor(sf::Color(105, 105, 105));
		mShape.setOutlineColor(sf::Color(0, 0, 0));
		mShape.setOutlineThickness(1.f);
	}

	void Button::setCallback(Callback callback)
	{
		mCallback = std::move(callback);
	}

	void Button::setText(const std::string &text)
	{
		mText.setString(text);
		centerOrigin(mText);
	}

	void Button::setSize(sf::Vector2f size)
	{
		mShape.setSize(size);
		centerOrigin(mShape);
	}

	void Button::setFontSize(unsigned int size)
	{
		mText.setCharacterSize(size);
		centerOrigin(mText);
	}

	bool Button::isSelectable() const
	{
		return true;
	}

	void Button::select()
	{
		Component::select();
		mText.setFillColor(sf::Color(255, 0, 0));
	}

	void Button::deselect()
	{
		Component::deselect();
		mText.setFillColor(sf::Color(255, 255, 255));
	}

	void Button::activate()
	{
		if (mCallback)
		{
			mCallback();
		}
	}

	void Button::handleEvent(const sf::Event &event)
	{
		if (event.type == sf::Event::MouseMoved)
		{
			int x = event.mouseMove.x, y = event.mouseMove.y;
			sf::FloatRect bounds = getWorldTransform().transformRect(mShape.getGlobalBounds());
			// std::cout << "Begin\nLeft: " << bounds.left << "\nTop: " << bounds.top << "\nWidth: " << bounds.width << "\nHeight" << bounds.height << std::endl;
			if ((x >= bounds.left) && (x <= bounds.left + bounds.width) &&
				(y >= bounds.top) && (y <= bounds.top + bounds.height))
			{
				if (!isSelected())
				{
					select();
				}
			}
			else if (isSelected())
			{
				deselect();
			}
		}
	}

	Component::Type Button::getType() const
	{
		return Component::Button;
	}

	void Button::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
	{
		target.draw(mShape, states);
		target.draw(mText, states);
	}
}