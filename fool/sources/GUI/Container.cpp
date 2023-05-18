#include "GUI/Container.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cassert>

namespace GUI
{
	Container::Container()
		: mChildren(),
		  mSelectedChild(-1)
	{
	}

	bool Container::isSelectable() const
	{
		return false;
	}

	void Container::handleEvent(const sf::Event &event)
	{
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		{
			for (auto &&element : mChildren)
			{
				if (element->getType() == Component::List)
				{
					element->handleEvent(event);
				}
				else if (element->getType() == Component::InputText && !element->isSelected() && element->isActive())
				{
					element->deactivate();
				}
			}

			if (hasSelection())
			{
				switch (mChildren[mSelectedChild]->getType())
				{
				case Component::Container:
					mChildren[mSelectedChild]->handleEvent(event);
					break;

				default:
					mChildren[mSelectedChild]->activate();
					break;
				}
			}
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			if (hasSelection())
			{
				mChildren[mSelectedChild]->handleEvent(event);
				switch (mChildren[mSelectedChild]->getType())
				{
				case Component::Container:
					if (!dynamic_cast<Container *>(mChildren[mSelectedChild].get())->hasSelection())
					{
						mSelectedChild = -1;
					}
					break;

				default:
					if (!mChildren[mSelectedChild]->isSelected())
					{
						mSelectedChild = -1;
					}
					break;
				}
			}
			// hasSelection may changed in previous if statement
			if (!hasSelection())
			{
				for (int i = 0; i < mChildren.size(); i++)
				{
					mChildren[i]->handleEvent(event);
					switch (mChildren[i]->getType())
					{
					case Component::Container:
						if (dynamic_cast<Container *>(mChildren[i].get())->hasSelection())
						{
							mSelectedChild = i;
						}
						break;

					default:
						if (mChildren[i]->isSelected())
						{
							mSelectedChild = i;
						}
						break;
					}
					if (hasSelection())
					{
						break;
					}
				}
			}
		}
		else
		{
			for (auto &&element : mChildren)
			{
				element->handleEvent(event);
			}
		}
	}

	// check if component is was selected
	void Container::attachChild(Component::Ptr child)
	{
		child->mParent = this;
		mChildren.push_back(std::move(child));
	}

	Component::Ptr Container::detachChild(const Component &node)
	{
		auto found = std::find_if(mChildren.begin(), mChildren.end(), [&](Component::Ptr &ptr) -> bool
								  { return ptr.get() == &node; });
		assert(found != mChildren.end());
		if (mChildren[mSelectedChild] == *found)
		{
			mSelectedChild = -1;
		}

		Component::Ptr result = std::move(*found);
		result->mParent = nullptr;
		mChildren.erase(found);
		return result;
	}

	bool Container::hasSelection() const
	{
		return mSelectedChild >= 0;
	}

	Component::Type Container::getType() const
	{
		return Component::Container;
	}

	void Container::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		drawCurrent(target, states);
		drawChildren(target, states);
	}

	void Container::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
	{
	}

	void Container::drawChildren(sf::RenderTarget &target, sf::RenderStates states) const
	{
		for (const Component::Ptr &child : mChildren)
		{
			child->draw(target, states);
		}
	}
}