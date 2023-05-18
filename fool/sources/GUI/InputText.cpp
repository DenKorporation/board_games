#include "GUI/InputText.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace GUI
{
	InputText::Style::Style(unsigned int fontSize = 16, sf::Color textColor = sf::Color(255, 255, 255), sf::Color fillColor = sf::Color(105, 105, 105),
							sf::Color outlineColor = sf::Color(0, 0, 0), float outlineThickness = 1)
		: fontSize(fontSize),
		  textColor(textColor),
		  fillColor(fillColor),
		  outlineColor(outlineColor),
		  outlineThickness(outlineThickness)
	{
	}

	InputText::InputText(FontHolder &fonts, Fonts::ID fontID)
		: mText("", fonts.get(fontID), 16),
		  mAlignment(Alignment::Left),
		  mActiveStyle(),
		  mNotActiveStyle(),
		  mOnEnter(),
		  mOnInput(),
		  mShape(),
		  mIsAlwaysActive(false)
	{
		applyStyle(mNotActiveStyle);
	}

	void InputText::setOnEnterAction(Callback callback)
	{
		mOnEnter = callback;
	}

	void InputText::setOnInputAction(InputCallback callback)
	{
		mOnInput = callback;
	}

	bool InputText::isSelectable() const
	{
		return true;
	}

	void InputText::select()
	{
		if (!mIsAlwaysActive)
		{
			Component::select();
			mShape.setOutlineColor(sf::Color::Blue);
		}
	}

	void InputText::deselect()
	{
		if (!mIsAlwaysActive)
		{
			Component::deselect();
			if (isActive())
			{
				applyStyle(mActiveStyle);
			}
			else
			{
				applyStyle(mNotActiveStyle);
			}
		}
	}

	void InputText::activate()
	{
		if (!mIsAlwaysActive)
		{
			Component::activate();
			applyStyle(mActiveStyle);
			if (isSelected())
			{
				mShape.setOutlineColor(sf::Color::Blue);
			}
		}
	}

	void InputText::deactivate()
	{
		if (!mIsAlwaysActive)
		{
			Component::deactivate();
			applyStyle(mNotActiveStyle);
		}
	}

	void InputText::setText(const sf::String &text)
	{
		mText.setString(text);
		setOrigin();
	}

	void InputText::setAlignment(Alignment alignment)
	{
		mAlignment = alignment;
		setOrigin();
	}

	void InputText::setSize(sf::Vector2f size)
	{
		mShape.setSize(size);
		setOrigin();
	}

	void InputText::setIsAlwaysActive(bool isAlwaysActive)
	{
		mIsAlwaysActive = isAlwaysActive;
		Component::deselect();
		Component::deactivate();
		if (mIsAlwaysActive)
		{
			Component::activate();
			applyStyle(mActiveStyle);
		}
		else
		{
			applyStyle(mNotActiveStyle);
		}
	}

	void InputText::setActiveStyle(Style style)
	{
		mActiveStyle = style;
		if (isActive())
		{
			applyStyle(mActiveStyle);
			if (isSelected())
			{
				mShape.setOutlineColor(sf::Color::Blue);
			}
		}
	}

	void InputText::setNotActiveStyle(Style style)
	{
		mNotActiveStyle = style;
		if (!isActive())
		{
			applyStyle(mNotActiveStyle);
			if (isSelected())
			{
				mShape.setOutlineColor(sf::Color::Blue);
			}
		}
	}

	sf::FloatRect InputText::getBounds() const
	{
		return getWorldTransform().transformRect(mShape.getGlobalBounds());
	}

	sf::String InputText::getString()
	{
		return mText.getString();
	}

	void InputText::handleEvent(const sf::Event &event)
	{
		if (event.type == sf::Event::TextEntered && isActive())
		{
			sf::String text = mText.getString();
			if (event.text.unicode == 8)
			{
				if (!text.isEmpty())
				{
					text.erase(text.getSize() - 1, 1);
				}
			}
			else if (event.text.unicode == 13 && mOnEnter)
			{
				mOnEnter();
			}
			else
			{
				if (mOnInput)
				{
					if (mOnInput(event.text.unicode))
					{
						text.insert(text.getSize(), sf::String(event.text.unicode));
					}
				}
				else
				{
					text.insert(text.getSize(), sf::String(event.text.unicode));
				}
			}
			mText.setString(text);
			setOrigin();
		}
		else if (event.type == sf::Event::MouseMoved && !mIsAlwaysActive)
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

	Component::Type InputText::getType() const
	{
		return Component::InputText;
	}

	void InputText::applyStyle(Style style)
	{
		mShape.setFillColor(style.fillColor);
		mShape.setOutlineColor(style.outlineColor);
		mShape.setOutlineThickness(style.outlineThickness);

		mText.setCharacterSize(style.fontSize);
		mText.setFillColor(style.textColor);

		setOrigin();
	}

	void InputText::setOrigin()
	{
		centerOrigin(mText);
		centerOrigin(mShape);
		switch (mAlignment)
		{
		case Left:
			mText.setOrigin(mShape.getLocalBounds().width / 2.f, mText.getOrigin().y);

			break;
		case Right:
			mText.setOrigin(mText.getLocalBounds().width - mShape.getLocalBounds().width / 2.f, mText.getOrigin().y);
			break;
		}
	}

	void InputText::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
	{
		target.draw(mShape, states);
		target.draw(mText, states);
	}
}