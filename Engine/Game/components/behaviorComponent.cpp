#include "behaviorComponent.h"
#include <Engine/Game/ai/behaviortree/btnode.h>

BehaviorComponent::BehaviorComponent()
	:GameComponent("behavior")
{

}

BehaviorComponent::BehaviorComponent(std::shared_ptr<BTNode> root)
	: GameComponent("behavior")
{
	setRoot(root);
}

void BehaviorComponent::start()
{
	root->start();
}

void BehaviorComponent::update(double seconds)
{
	if (root != nullptr) root->update(seconds);
}

void BehaviorComponent::setRoot(std::shared_ptr<BTNode> root)
{
	this->root = root;
	root->setBehaviorComponent(shared_from_this());
}