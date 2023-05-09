#pragma once

#include "ListItem.h"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Sound.hpp>

namespace GUI
{
	class OnlineGameItem : public ListItem
	{
	public:
		typedef std::unique_ptr<OnlineGameItem> Ptr;

		struct Style
		{
			Style(unsigned int mainFontSize, sf::Color mainTextColor,
				  unsigned int addFontSize, sf::Color addTextColor,
				  sf::Color fillColor, sf::Color outlineColor, float outlineThickness);
			unsigned int mainFontSize;
			unsigned int addFontSize;
			sf::Color mainTextColor;
			sf::Color addTextColor;
			sf::Color fillColor;
			sf::Color outlineColor;
			float outlineThickness;
		};

	public:
		OnlineGameItem(Offset margin, const FontHolder &fonts, const SoundHolder &sounds);

		std::any getData() const;
		sf::FloatRect getMinBounds() const;
		sf::FloatRect getFullBounds() const;

		virtual void select();

		void setData(std::string data);
		void setName(const sf::String &text);
		void setId(const std::string &text);
		void setSize(sf::Vector2f size);

		void applyNormalStyle();
		void applyHoverStyle();
		void applySelectedStyle();

		void setNormalStyle(Style style);
		void setSelectedStyle(Style style);
		void setHoverStyle(Style style);

	private:
		void applyStyle(Style style);

		void setTextOrigin();
		void setRectOrigin();

		void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		Style hoverStyle;
		Style normalStyle;
		Style selectedStyle;

		std::string data;

		sf::Sound mSelectSound;

		sf::RectangleShape mShape;
		sf::Text mTextName;
		sf::Text mTextId;
	};
}
