#pragma once

#include "Engine/SceneNode.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include <memory>

class ShapeNode : public SceneNode
{
	public:
		typedef std::unique_ptr<ShapeNode> Ptr;

	public:
		ShapeNode(sf::Vector2f size);

		void setFillColor(sf::Color color);
		void setOutlineColor(sf::Color color);
		void setOutlineThickness(float thickness);
		void setSize(sf::Vector2f size);

	private:
		virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		sf::RectangleShape mShape;
};