#pragma once

#include <vector>
#include <Engine/Game/collision/gridnode.h>

typedef std::pair<std::shared_ptr<CollisionComponent>, std::shared_ptr<CollisionResponseComponent>> entityComponentPair;

class HierarchicalGrid {
public:
	HierarchicalGrid(int level, std::shared_ptr<AABB> aabb);
	void split(std::shared_ptr<GridNode> node);
	bool insert(int id, std::shared_ptr<entityComponentPair> entity, std::shared_ptr<AABB> aabb);
	void update(std::shared_ptr<entityComponentPair> entity);
	void collide();
	void collide(std::shared_ptr<GridNode> node, std::shared_ptr<GridNode> nextNode);

	void print();

private:
	int level;
	int nodesNum;
	std::vector<std::shared_ptr<GridNode>> nodes;

};