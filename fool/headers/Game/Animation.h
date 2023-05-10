#pragma once

#include "Engine/SceneNode.h"
#include "Engine/Resource/ResourceIdentifiers.h"
#include "Engine/Resource/ResourceHolder.hpp"
#include "Game/CardGroup.h"
#include "Game/CardDeck.h"
#include "Game/CardField.h"

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Audio/Sound.hpp>

class Animation
{
public:
	Animation(Card *node, SceneNode *parent, SceneNode *destination, sf::Vector2f startPostion, sf::Vector2f endPosition, sf::Time animationTime, const SoundHolder &sounds);
	Animation(Card *node, SceneNode *parent, SceneNode *destination, CardField::Type type, sf::Vector2f startPostion, sf::Vector2f endPosition, sf::Time animationTime, const SoundHolder &sounds);
	void release();
	bool update(const sf::Time dt);
	void setDelayTime(const sf::Time delay);

private:
	SceneNode *mParent;
	Card *mNode;
	SceneNode *mDestination;
	CardField::Type mType;

	bool isSoundStarted;
	sf::Sound mSound;

	sf::Vector2f mVelocity;
	sf::Time mDelayTime;
	sf::Time mElapsedTime;
	sf::Time mAnimationTime;
};