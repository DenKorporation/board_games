#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>
#include <vector>

class CardGroup;
class CardField;
class CardDeck;

namespace GUI
{
	class Container;
}

class SceneNode : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
{
	friend class CardGroup;
	friend class CardField;
	friend class CardDeck;
	friend class GUI::Container;

public:
	typedef std::unique_ptr<SceneNode> Ptr;

public:
	SceneNode();

	virtual void attachChild(Ptr child);
	virtual Ptr detachChild(const SceneNode &node);

	sf::Vector2f getWorldPosition() const;
	sf::Transform getWorldTransform() const;

	virtual void handleEvent(const sf::Event &event);

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
	virtual void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	std::vector<Ptr> mChildren;
	SceneNode *mParent;
};
