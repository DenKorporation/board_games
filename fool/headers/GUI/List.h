#pragma once

#include "Component.h"
#include "ListItem.h"
#include "Offset.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include <vector>
#include <memory>
#include <any>

namespace GUI
{

	class List : public Component
	{
	public:
		typedef std::unique_ptr<List> Ptr;

	public:
		List(sf::Vector2f size, Offset margin);
		void pack(ListItem::Ptr component);
		void detachAllListItem();

		virtual bool isSelectable() const;
		virtual void handleEvent(const sf::Event &event);

		void setFillColor(sf::Color color);
		void setOutlineColor(sf::Color color);
		void setOutlineThickness(float thickness);

		std::any getSelectedData();

		bool hasHover() const;
		bool hasSelection() const;
		virtual Component::Type getType() const;

	private:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
		virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
		virtual void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		static const float ScrollRatio;
		sf::Vector2f mScrollTranslation;

		sf::Vector2f mBoundSize;
		float mCurHeight;
		Offset mMargin;
		std::vector<ListItem::Ptr> mChildren;
		int mHoveredChild;
		int mSelectedChild;

		sf::RectangleShape mBackgroundShape;
	};

}