#include "OnlineGameItem.h"
#include "Utility.h"

#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

namespace GUI
{
	OnlineGameItem::Style::Style(unsigned int fontSize = 16, sf::Color textColor = sf::Color(255, 255, 255), sf::Color fillColor = sf::Color(105, 105, 105),
								 sf::Color outlineColor = sf::Color(0, 0, 0), float outlineThickness = 1)
		: fontSize(fontSize),
		  textColor(textColor),
		  fillColor(fillColor),
		  outlineColor(outlineColor),
		  outlineThickness(outlineThickness)
	{
	}

	OnlineGameItem::OnlineGameItem(Offset margin, const FontHolder &fonts, const SoundHolder &sounds)
		: ListItem(margin),
		  mText("", fonts.get(Fonts::Main), 16),
		  data(),
		  mShape(),
		  normalStyle(),
		  hoverStyle(),
		  selectedStyle()
	{
		mSelectSound.setBuffer(sounds.get(Sounds::ButtonSelect));
		applyStyle(normalStyle);
	}

	std::any OnlineGameItem::getData() const
	{
		return data;
	}

	sf::FloatRect OnlineGameItem::getMinBounds() const
	{
		// auto bound = getTransform().transformRect(mShape.getGlobalBounds());
		// std::cout << "Begin\nLeft: " << bound.left << "\nTop: " << bound.top << "\nWidth: " << bound.width << "\nHeight:" << bound.height << std::endl;
		auto bound = mShape.getGlobalBounds();
		float thick = mShape.getOutlineThickness();
		bound.left += thick;
		bound.top += thick;
		bound.width -= 2 * thick;
		bound.height -= 2 * thick;

		return bound;
	}

	sf::FloatRect OnlineGameItem::getFullBounds() const
	{
		// auto bound = getWorldTransform().transformRect(mShape.getGlobalBounds());
		auto bound = mShape.getGlobalBounds();
		float thick = mShape.getOutlineThickness();
		bound.left -= margin.left - thick;
		bound.top -= margin.top - thick;
		bound.width += margin.left + margin.right - 2 * thick;
		bound.height += margin.top + margin.bottom - 2 * thick;

		return bound;
	}

	void OnlineGameItem::select()
	{
		Component::select();
		mSelectSound.play();
	}

	void OnlineGameItem::setData(std::string data)
	{
		this->data = data;
	}

	void OnlineGameItem::setText(const std::string &text)
	{
		mText.setString(text);
		setTextOrigin();
	}

	void OnlineGameItem::setSize(sf::Vector2f size)
	{
		size.x -= (margin.left + margin.right);
		size.y -= (margin.top + margin.bottom);
		mShape.setSize(size);

		setTextOrigin();
		setRectOrigin();
	}

	void OnlineGameItem::applyNormalStyle()
	{
		applyStyle(normalStyle);
	}

	void OnlineGameItem::applyHoverStyle()
	{
		applyStyle(hoverStyle);
	}

	void OnlineGameItem::applySelectedStyle()
	{
		applyStyle(selectedStyle);
	}

	void OnlineGameItem::setNormalStyle(Style style)
	{
		normalStyle = style;
		applyNormalStyle();
	}

	void OnlineGameItem::setSelectedStyle(Style style)
	{
		selectedStyle = style;
	}

	void OnlineGameItem::setHoverStyle(Style style)
	{
		hoverStyle = style;
	}

	void OnlineGameItem::applyStyle(Style style)
	{
		mShape.setFillColor(style.fillColor);
		mShape.setOutlineColor(style.outlineColor);
		mShape.setOutlineThickness(style.outlineThickness);

		mText.setCharacterSize(style.fontSize);
		mText.setFillColor(style.textColor);

		setTextOrigin();
		setRectOrigin();
	}

	void OnlineGameItem::setTextOrigin()
	{
		centerOrigin(mText);
		mText.setOrigin(mText.getOrigin().x, mText.getOrigin().y - mShape.getGlobalBounds().height / 2.f - margin.top);
	}

	void OnlineGameItem::setRectOrigin()
	{
		centerOrigin(mShape);
		mShape.setOrigin(mShape.getOrigin().x, mShape.getOrigin().y - mShape.getGlobalBounds().height / 2.f - margin.top);
	}

	void OnlineGameItem::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
	{
		states.transform.translate(mScrollTranslation);
		target.draw(mShape, states);
		target.draw(mText, states);
	}
}