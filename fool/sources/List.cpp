#include "List.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <algorithm>

namespace GUI
{
	const float List::ScrollRatio = 100;

	List::List(sf::Vector2f size, Offset margin)
		: mChildren(),
		  mHoveredChild(-1),
		  mSelectedChild(-1),
		  mBoundSize(size),
		  mMargin(margin),
		  mCurHeight(0),
		  mScrollTranslation()
	{
		size.x -= (margin.left + margin.right);
		size.y -= (margin.top + margin.bottom);
		mBackgroundShape.setSize(size);
		mBackgroundShape.setOrigin(std::floor(mBackgroundShape.getLocalBounds().width / 2.f), -margin.top);
	}

	void List::pack(ListItem::Ptr component)
	{
		component->setPosition(0, mMargin.top + mCurHeight);
		mCurHeight += component->getFullBounds().height;
		component->setSize(sf::Vector2f(mBoundSize.x - mMargin.left - mMargin.right, component->getFullBounds().height));
		component->applyNormalStyle();
		component->mParent = this;
		mChildren.push_back(std::move(component));
	}

	bool List::isSelectable() const
	{
		return false;
	}

	void List::handleEvent(const sf::Event &event)
	{
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		{
			if (hasHover())
			{
				if (mHoveredChild != mSelectedChild)
				{
					if (hasSelection())
					{
						mChildren[mSelectedChild]->applyNormalStyle();
					}
					mSelectedChild = mHoveredChild;
					mChildren[mSelectedChild]->applySelectedStyle();
				}
			}
			// else if (hasSelection())
			// {
			// 	mChildren[mSelectedChild]->applyNormalStyle();
			// 	mSelectedChild = -1;
			// }
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			if (hasHover())
			{
				mChildren[mHoveredChild]->handleEvent(event);
				if (!mChildren[mHoveredChild]->isSelected())
				{
					if (mSelectedChild != mHoveredChild)
					{
						mChildren[mHoveredChild]->applyNormalStyle();
					}
					mHoveredChild = -1;
				}
			}
			// hasSelection may changed in previous if statement
			if (!hasHover())
			{
				for (int i = 0; i < mChildren.size(); i++)
				{
					mChildren[i]->handleEvent(event);
					if (mChildren[i]->isSelected())
					{
						mHoveredChild = i;
						if (mSelectedChild != mHoveredChild)
						{
							mChildren[mHoveredChild]->applyHoverStyle();
						}
						break;
					}
				}
			}
		}
		else if (event.type == sf::Event::MouseWheelScrolled)
		{
			float x = event.mouseWheelScroll.x, y = event.mouseWheelScroll.y;
			auto bounds = getWorldTransform().transformRect(mBackgroundShape.getGlobalBounds());
			if ((x >= bounds.left) && (x <= bounds.left + bounds.width) &&
				(y >= bounds.top) && (y <= bounds.top + bounds.height))
			{
				float delta = event.mouseWheelScroll.delta;
				mScrollTranslation.y += delta * ScrollRatio;
				float maxScrollDown = mBoundSize.y - mMargin.bottom - mMargin.top - mCurHeight;
				mScrollTranslation.y = std::min(0.f, std::max(maxScrollDown, mScrollTranslation.y));
				for (auto &&element : mChildren)
				{
					element->setScrollTranslation(mScrollTranslation);
				}
			}
		}
	}

	void List::setFillColor(sf::Color color)
	{
		mBackgroundShape.setFillColor(color);
	}

	void List::setOutlineColor(sf::Color color)
	{
		mBackgroundShape.setOutlineColor(color);
	}

	void List::setOutlineThickness(float thickness)
	{
		mBackgroundShape.setOutlineThickness(thickness);
	}

	std::any List::getSelectedData()
	{
		if (!hasSelection())
		{
			return nullptr;
		}

		return mChildren[mSelectedChild]->getData();
	}

	Component::Type List::getType() const
	{
		return Component::List;
	}

	bool List::hasHover() const
	{
		return mHoveredChild >= 0;
	}

	bool List::hasSelection() const
	{
		return mSelectedChild >= 0;
	}

	void
	List::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		drawCurrent(target, states);
		drawChildren(target, states);
	}

	void List::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
	{
		target.draw(mBackgroundShape, states);
	}

	void List::drawChildren(sf::RenderTarget &target, sf::RenderStates states) const
	{
		for (const auto &child : mChildren)
		{
			child->draw(target, states);
		}
	}

	void List::detachAllListItem()
	{
		mScrollTranslation = sf::Vector2f(0, 0);
		mCurHeight = 0;
		mHoveredChild = -1;
		mSelectedChild = -1;
		mChildren.clear();
	}
}