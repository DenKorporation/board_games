#include "GUI/OnlineGameItem.h"
#include "Utility.h"

#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

namespace GUI
{
	OnlineGameItem::Style::Style(unsigned int mainFontSize = 16, sf::Color mainTextColor = sf::Color::White,
								 unsigned int addFontSize = 8, sf::Color addTextColor = sf::Color::White,
								 sf::Color fillColor = sf::Color(105, 105, 105), sf::Color outlineColor = sf::Color::Black, float outlineThickness = 1)
		: mainFontSize(mainFontSize),
		  mainTextColor(mainTextColor),
		  addFontSize(addFontSize),
		  addTextColor(addTextColor),
		  fillColor(fillColor),
		  outlineColor(outlineColor),
		  outlineThickness(outlineThickness)
	{
	}

	OnlineGameItem::OnlineGameItem(Offset margin, const FontHolder &fonts, const SoundHolder &sounds)
		: ListItem(margin),
		  mTextName("", fonts.get(Fonts::Russian)),
		  mTextId("", fonts.get(Fonts::Main)),
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

	void OnlineGameItem::setName(const sf::String &text)
	{
		mTextName.setString(text);
		setTextOrigin();
	}

	void OnlineGameItem::setId(const std::string &text)
	{
		mTextId.setString(text);
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

		mTextName.setCharacterSize(style.mainFontSize);
		mTextName.setFillColor(style.mainTextColor);

		mTextId.setCharacterSize(style.addFontSize);
		mTextId.setFillColor(style.addTextColor);

		setTextOrigin();
		setRectOrigin();
	}

	void OnlineGameItem::setTextOrigin()
	{
		sf::FloatRect bounds = mTextName.getLocalBounds();
		mTextName.setOrigin(std::floor(bounds.width / 2.f), std::floor((bounds.height + bounds.top) * 1.45f / 2.f) - mShape.getGlobalBounds().height / 3.f - margin.top);

		sf::FloatRect IdBounds = mTextId.getLocalBounds();
		mTextId.setOrigin(std::floor(IdBounds.width / 2.f), std::floor((IdBounds.height + IdBounds.top) * 1.45f / 2.f) - mShape.getGlobalBounds().height * 2.f / 3.f - margin.top);
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
		target.draw(mTextName, states);
		target.draw(mTextId, states);
	}
}