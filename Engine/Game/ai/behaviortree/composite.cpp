#include "composite.h"

void Composite::start()
{
	for (auto child : children) child->start();
}

void Composite::addChild(std::shared_ptr<BTNode> child)
{
	children.push_back(child);
}

void Composite::setBehaviorComponent(std::shared_ptr<BehaviorComponent> behaviorComponent)
{
	this->behaviorComponent = behaviorComponent;
	for (auto child : children) child->setBehaviorComponent(behaviorComponent);
}