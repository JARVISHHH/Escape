#pragma once

#include <vector>
#include <Engine/Game/collision/gridnode.h>

typedef std::pair<std::shared_ptr<CollisionComponent>, std::shared_ptr<CollisionResponseComponent>> entityComponentPair;

class HierarchicalGrid {
public:
	HierarchicalGrid(int level);
	void split(std::shared_ptr<GridNode> node);
	void build(std::vector<entityComponentPair> entityComponentPairs);
	bool insert(std::shared_ptr<GridNode> node, entityComponentPair entity, std::shared_ptr<AABB> aabb);
	void collide();
	void collide(std::shared_ptr<GridNode> node, std::shared_ptr<GridNode> nextNode);

private:
	int level;
	int nodesNum;
	std::vector<std::shared_ptr<AABB>> aabbs;
	std::vector<std::shared_ptr<GridNode>> nodes;

};