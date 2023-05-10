#include "GUI/ListItem.h"
#include "GUI/Offset.h"

namespace GUI
{
	ListItem::ListItem(Offset margin) : margin(margin), mScrollTranslation()
	{
	}

	bool ListItem::isSelectable() const
	{
		return true;
	}

	void ListItem::handleEvent(const sf::Event &event)
	{
		if (event.type == sf::Event::MouseMoved)
		{
			int x = event.mouseMove.x, y = event.mouseMove.y;
			sf::FloatRect bounds = getWorldTransform().transformRect(getMinBounds());

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

	Component::Type ListItem::getType() const
	{
		return Component::ListItem;
	}

	sf::Transform ListItem::getWorldTransform() const
	{
		sf::Transform transform = SceneNode::getWorldTransform();
		return transform.translate(mScrollTranslation);
	}

	void ListItem::setScrollTranslation(sf::Vector2f translation)
	{
		mScrollTranslation = translation;
	}
}