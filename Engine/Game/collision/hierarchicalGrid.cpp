#include "hierarchicalGrid.h"

#include "Engine/Game/gameObject.h"
#include "Engine/Game/components/collisionComponents/collisionComponent.h"
#include "Engine/Game/components/collisionResponseComponent.h"
#include <Engine/Game/components/transformComponent.h>

HierarchicalGrid::HierarchicalGrid(int level, std::shared_ptr<AABB> aabb)
	:level(level)
{
	nodesNum = std::pow(2, level);
	nodes.resize(nodesNum);
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i] = std::make_shared<GridNode>(i);
	}
	nodes[0]->aabb = aabb;

	nodes[1]->aabb = nodes[0]->aabb;
	split(nodes[1]);
}

HierarchicalGrid::~HierarchicalGrid()
{
	//std::cout << "HierarchicalGrid delete grid nodes: " << nodesNum << std::endl;
}

void HierarchicalGrid::split(std::shared_ptr<GridNode> node)
{
	node->entityComponentPairs.clear();

	int id = node->id;
	if (2 * id >= nodesNum) return;

	float differences[3];
	for (int i = 0; i < 3; i++) {
		differences[i] = node->aabb->getMaxPoint()[i] - node->aabb->getMinPoint()[i];
	}

	nodes[id * 2]->aabb->setMinPoint(node->aabb->getMinPoint());
	nodes[id * 2 + 1]->aabb->setMaxPoint(node->aabb->getMaxPoint());

	if (differences[0] >= differences[1] && differences[0] >= differences[2]) {
		nodes[id * 2]->aabb->setMaxPoint({node->aabb->getCenter()[0], node->aabb->getMaxPoint()[1], node->aabb->getMaxPoint()[2], 1});
		nodes[id * 2 + 1]->aabb->setMinPoint({ node->aabb->getCenter()[0], node->aabb->getMinPoint()[1], node->aabb->getMinPoint()[2], 1 });
	}
	else if (differences[1] >= differences[0] && differences[1] >= differences[2]) {
		nodes[id * 2]->aabb->setMaxPoint({ node->aabb->getMaxPoint()[0], node->aabb->getCenter()[1], node->aabb->getMaxPoint()[2], 1 });
		nodes[id * 2 + 1]->aabb->setMinPoint({ node->aabb->getMinPoint()[0], node->aabb->getCenter()[1], node->aabb->getMinPoint()[2], 1 });
	}
	else {
		nodes[id * 2]->aabb->setMaxPoint({ node->aabb->getMaxPoint()[0], node->aabb->getMaxPoint()[1], node->aabb->getCenter()[2], 1 });
		nodes[id * 2 + 1]->aabb->setMinPoint({ node->aabb->getMinPoint()[0], node->aabb->getMinPoint()[1], node->aabb->getCenter()[2], 1 });
	}

	split(nodes[id * 2]);
	split(nodes[id * 2 + 1]);
}

bool HierarchicalGrid::insert(int id, std::shared_ptr<entityComponentPair> entity, std::shared_ptr<AABB> aabb)
{
	if (id >= nodesNum) return false;

	auto node = nodes[id];

	for (int i = 0; i < 3; i++) {
		if (node->aabb->getMinPoint()[i] > aabb->getMinPoint()[i]) return false;
		if (node->aabb->getMaxPoint()[i] < aabb->getMaxPoint()[i]) return false;
	}

	if (id * 2 < nodesNum && insert(id * 2, entity, aabb)) return true;
	if (id * 2 + 1 < nodesNum && insert(id * 2 + 1, entity, aabb)) return true;

	node->entityComponentPairs.insert(entity);
	entity->first->gridNode = node;

	//std::cout << "id: " << id << std::endl;
	//aabb->printMaxPoint();
	//aabb->printMinPoint();

	return true;
}

void HierarchicalGrid::update(std::shared_ptr<entityComponentPair> entity)
{
	const auto& ray = entity->first->getGameObject()->getComponent<TransformComponent>("transform")->getRay();
	//entity->first->getAABB()->printMaxPoint();
	//entity->first->getAABB()->printMinPoint();
	//std::cout << glm::length(ray->direction) << std::endl;
	if (glm::length(ray->direction) == 0) return;  // No move
	if(entity->first->gridNode.lock() != nullptr)
		entity->first->gridNode.lock()->entityComponentPairs.erase(entity);
	if (!insert(1, entity, entity->first->getAABB()))
		std::cout << "grid insert failed" << std::endl;
}

int sum = 0;

void HierarchicalGrid::collide()
{
	//sum = 0;
	//print();
	for (int i = 1; i < nodesNum; i++) {
		collide(nodes[i], nodes[i]);
	}
	//std::cout << "collision: " << sum << std::endl;
}

void HierarchicalGrid::collide(std::shared_ptr<GridNode> node, std::shared_ptr<GridNode> nextNode)
{
	if (node->entityComponentPairs.size() == 0) return;
	node->collide(nextNode);
	if (nextNode->id * 2 < nodesNum) collide(node, nodes[nextNode->id * 2]);
	if (nextNode->id * 2 + 1 < nodesNum) collide(node, nodes[nextNode->id * 2 + 1]);
}

void HierarchicalGrid::collideAll(std::shared_ptr<HierarchicalGrid> grid1, std::shared_ptr<HierarchicalGrid> grid2, int nodeIndex1, int nodeIndex2) {
	if (nodeIndex1 >= grid1->nodesNum) return;
	if (nodeIndex2 >= grid2->nodesNum) return;
	if (grid1 == grid2) grid1->collide();
	else {
		grid1->nodes[nodeIndex1]->collide(grid2->nodes[nodeIndex2]);
		collide(grid1, grid2, nodeIndex1, nodeIndex2 * 2);
		collide(grid1, grid2, nodeIndex1, nodeIndex2 * 2 + 1);
		collide(grid2, grid1, nodeIndex2, nodeIndex1 * 2);
		collide(grid2, grid1, nodeIndex2, nodeIndex1 * 2 + 1);
		collideAll(grid1, grid2, nodeIndex1 * 2, nodeIndex2 * 2);
		collideAll(grid1, grid2, nodeIndex1 * 2 + 1, nodeIndex2 * 2 + 1);
	}
}

void HierarchicalGrid::collide(std::shared_ptr<HierarchicalGrid> grid1, std::shared_ptr<HierarchicalGrid> grid2, int nodeIndex1, int nodeIndex2)
{
	if (nodeIndex1 >= grid1->nodesNum) return;
	if (nodeIndex2 >= grid2->nodesNum) return;
	grid1->nodes[nodeIndex1]->collide(grid2->nodes[nodeIndex2]);
	collide(grid1, grid2, nodeIndex1, nodeIndex2 * 2);
	collide(grid1, grid2, nodeIndex1, nodeIndex2 * 2 + 1);
}

void HierarchicalGrid::print()
{
	for (int i = 0; i < nodes.size(); i++) {
		std::cout << "id: " << nodes[i]->id << " num: " << nodes[i]->entityComponentPairs.size() << std::endl;
	}
}