#include "SpriteNode.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

SpriteNode::SpriteNode(const sf::Texture &texture) : mSprite(texture)
{
	centerOrigin(mSprite);
}

SpriteNode::SpriteNode(const sf::Texture &texture, const sf::IntRect &textureRect) : mSprite(texture, textureRect)
{
	centerOrigin(mSprite);
}

void SpriteNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}
