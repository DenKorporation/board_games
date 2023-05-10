#include "Engine/ShapeNode.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

ShapeNode::ShapeNode(sf::Vector2f size) : mShape(size)
{
	centerOrigin(mShape);
}

void ShapeNode::setFillColor(sf::Color color)
{
	mShape.setFillColor(color);
}

void ShapeNode::setOutlineColor(sf::Color color)
{
	mShape.setOutlineColor(color);
}

void ShapeNode::setOutlineThickness(float thickness)
{
	mShape.setOutlineThickness(thickness);
	centerOrigin(mShape);
}

void ShapeNode::setSize(sf::Vector2f size)
{
	mShape.setSize(size);
	centerOrigin(mShape);
}

void ShapeNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(mShape, states);
}
