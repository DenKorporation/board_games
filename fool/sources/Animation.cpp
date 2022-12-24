#include "Animation.h"

Animation::Animation(Card *node, SceneNode *parent, SceneNode *destination, sf::Vector2f startPosition, sf::Vector2f endPosition, sf::Time animationTime, const SoundHolder &sounds)
	: mNode(node),
	  mParent(parent),
	  mDestination(destination),
	  mVelocity((endPosition - startPosition) / animationTime.asSeconds()),
	  mAnimationTime(animationTime),
	  isSoundStarted(false)
{
	mSound.setBuffer(sounds.get(Sounds::CardFlip));
	node->setPosition(startPosition);
}
Animation::Animation(Card *node, SceneNode *parent, SceneNode *destination, CardField::Type type, sf::Vector2f startPosition, sf::Vector2f endPosition, sf::Time animationTime, const SoundHolder &sounds)
	: mNode(node),
	  mParent(parent),
	  mDestination(destination),
	  mVelocity((endPosition - startPosition) / animationTime.asSeconds()),
	  mAnimationTime(animationTime),
	  mType(type),
	  isSoundStarted(false)
{
	mSound.setBuffer(sounds.get(Sounds::CardFlip));
	node->setPosition(startPosition);
}

void Animation::release()
{
	mParent->detachChild(*mNode).release();
	Card::Ptr result(mNode);
	if (dynamic_cast<CardField *>(mDestination) != nullptr)
	{
		static_cast<CardField *>(mDestination)->pushCard(std::move(result), mType);
	}
	else if (dynamic_cast<CardGroup *>(mDestination) != nullptr)
	{
		static_cast<CardGroup *>(mDestination)->pushCard(std::move(result));
	}
	else if (dynamic_cast<CardPile *>(mDestination) != nullptr)
	{
		static_cast<CardPile *>(mDestination)->pushCard(std::move(result));
	}
}

bool Animation::update(const sf::Time dt)
{
	mElapsedTime += dt;
	if (mElapsedTime > mDelayTime)
	{
		if (!isSoundStarted)
		{
			isSoundStarted = true;
			mSound.play();
		}

		mNode->move(std::min(dt.asSeconds(), (mElapsedTime - mDelayTime).asSeconds()) * mVelocity);
	}
	if (mElapsedTime < mAnimationTime + mDelayTime)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Animation::setDelayTime(const sf::Time delay)
{
	mDelayTime = delay;
}
