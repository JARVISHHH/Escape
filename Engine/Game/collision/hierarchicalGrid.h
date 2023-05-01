#pragma once

#include <vector>
#include <Engine/Game/collision/gridnode.h>

typedef std::pair<std::shared_ptr<CollisionComponent>, std::shared_ptr<CollisionResponseComponent>> entityComponentPair;

class HierarchicalGrid {
public:
	HierarchicalGrid(int level, std::shared_ptr<AABB> aabb);
	~HierarchicalGrid();

	void split(std::shared_ptr<GridNode> node);
	bool insert(int id, std::shared_ptr<entityComponentPair> entity, std::shared_ptr<AABB> aabb);
	void update(std::shared_ptr<entityComponentPair> entity);
	void collide();
	void collide(std::shared_ptr<GridNode> node, std::shared_ptr<GridNode> nextNode);

	static void collideAll(std::shared_ptr<HierarchicalGrid> grid1, std::shared_ptr<HierarchicalGrid> grid2, int nodeIndex1, int nodeIndex2);
	static void collide(std::shared_ptr<HierarchicalGrid> grid1, std::shared_ptr<HierarchicalGrid> grid2, int nodeIndex1, int nodeIndex2);

	void print();

private:
	int level;
	int nodesNum;
	std::vector<std::shared_ptr<GridNode>> nodes;
};