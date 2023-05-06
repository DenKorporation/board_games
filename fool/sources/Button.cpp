#include "Button.h"
#include "Utility.h"

#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

namespace GUI
{
	Button::Style::Style(unsigned int fontSize = 16, sf::Color textColor = sf::Color(255, 255, 255), sf::Color fillColor = sf::Color(105, 105, 105),
						 sf::Color outlineColor = sf::Color(0, 0, 0), float outlineThickness = 1)
		: fontSize(fontSize),
		  textColor(textColor),
		  fillColor(fillColor),
		  outlineColor(outlineColor),
		  outlineThickness(outlineThickness)
	{
	}

	Button::Button(const FontHolder &fonts, const SoundHolder &sounds)
		: mText("", fonts.get(Fonts::Main), 16),
		  mCallback(),
		  mShape(),
		  mNormalStyle(),
		  mSelectedStyle()
	{
		mSelectSound.setBuffer(sounds.get(Sounds::ButtonSelect));
		applyStyle(mNormalStyle);
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

	void Button::clearTextAndCallback()
	{
		setCallback([this]() {});
		setText("");
	}

	bool Button::isSelectable() const
	{
		return true;
	}

	void Button::select()
	{
		Component::select();
		applyStyle(mSelectedStyle);
		mSelectSound.play();
	}

	void Button::deselect()
	{
		Component::deselect();
		applyStyle(mNormalStyle);
	}

	void Button::activate()
	{
		if (mCallback)
		{
			mCallback();
		}
	}

	void Button::setNormalStyle(Style style)
	{
		mNormalStyle = style;
		if (!isSelected())
		{
			applyStyle(mNormalStyle);
		}
	}

	void Button::setSelectedStyle(Style style)
	{
		mSelectedStyle = style;
		if (isSelected())
		{
			applyStyle(mSelectedStyle);
		}
	}

	void Button::handleEvent(const sf::Event &event)
	{
		if (event.type == sf::Event::MouseMoved)
		{
			int x = event.mouseMove.x, y = event.mouseMove.y;
			sf::FloatRect bounds = getWorldTransform().transformRect(mShape.getGlobalBounds());
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

	void Button::applyStyle(Style style)
	{
		mShape.setFillColor(style.fillColor);
		mShape.setOutlineColor(style.outlineColor);
		mShape.setOutlineThickness(style.outlineThickness);

		mText.setCharacterSize(style.fontSize);
		mText.setFillColor(style.textColor);

		centerOrigin(mShape);
		centerOrigin(mText);
	}

	void Button::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
	{
		target.draw(mShape, states);
		target.draw(mText, states);
	}
}