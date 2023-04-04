#include "hierarchicalGrid.h"

#include "Engine/Game/gameObject.h"
#include "Engine/Game/components/collisionComponents/collisionComponent.h"
#include "Engine/Game/components/collisionResponseComponent.h"

HierarchicalGrid::HierarchicalGrid(int level)
	:level(level)
{
	nodesNum = std::pow(2, level);
	nodes.resize(nodesNum);
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i] = std::make_shared<GridNode>(i);
	}
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

bool printRes = true;

void HierarchicalGrid::build(std::vector<entityComponentPair> entityComponentPairs)
{
	if (entityComponentPairs.size() == 0) return;

	aabbs.clear();
	aabbs.resize(entityComponentPairs.size());

	glm::vec4 maxPoint, minPoint;
	for (int i = 0; i < entityComponentPairs.size(); i++) {
		aabbs[i] = entityComponentPairs[i].first->getAABB();
	}
	maxPoint = aabbs[0]->getMaxPoint(), minPoint = aabbs[0]->getMinPoint();
	for (int i = 0; i < aabbs.size(); i++) {
		for (int j = 0; j < 3; j++) {
			maxPoint[j] = std::max(aabbs[i]->getMaxPoint()[j], maxPoint[j]);
			minPoint[j] = std::min(aabbs[i]->getMinPoint()[j], minPoint[j]);
		}
	}
	nodes[1]->aabb->setMaxPoint(maxPoint + glm::vec4(0.1, 0.1, 0.1, 0)), nodes[1]->aabb->setMinPoint(minPoint - glm::vec4(0.1, 0.1, 0.1, 0));
	split(nodes[1]);

	for (int i = 0; i < entityComponentPairs.size(); i++) {
		insert(nodes[1], entityComponentPairs[i], entityComponentPairs[i].first->getAABB());
	}

	if (printRes) {
		for (int i = 1; i < nodesNum; i++) {
			std::cout << "id: " << i << " num: " << nodes[i]->entityComponentPairs.size() << std::endl;
			std::cout << "max point: " << nodes[i]->aabb->getMaxPoint()[0] << " " << nodes[i]->aabb->getMaxPoint()[1] << " " << nodes[i]->aabb->getMaxPoint()[2] << std::endl;
			std::cout << "min point: " << nodes[i]->aabb->getMinPoint()[0] << " " << nodes[i]->aabb->getMinPoint()[1] << " " << nodes[i]->aabb->getMinPoint()[2] << std::endl;
		}
		printRes = false;
	}
}

bool HierarchicalGrid::insert(std::shared_ptr<GridNode> node, entityComponentPair entity, std::shared_ptr<AABB> aabb)
{
	for (int i = 0; i < 3; i++) {
		if (node->aabb->getMinPoint()[i] > aabb->getMinPoint()[i]) return false;
		if (node->aabb->getMaxPoint()[i] < aabb->getMaxPoint()[i]) return false;
	}
	
	int id = node->id;
	if (id * 2 < nodesNum && insert(nodes[id * 2], entity, aabb)) return true;
	if (id * 2 + 1 < nodesNum && insert(nodes[id * 2 + 1], entity, aabb)) return true;

	node->entityComponentPairs.push_back(entity);

	return true;
}

int sum = 0;

void HierarchicalGrid::collide()
{
	sum = 0;
	for (int i = 1; i < nodesNum; i++) {
		collide(nodes[i], nodes[i]);
	}
	std::cout << "collision: " << sum << std::endl;
}

void HierarchicalGrid::collide(std::shared_ptr<GridNode> node, std::shared_ptr<GridNode> nextNode)
{
	if (node->entityComponentPairs.size() == 0) return;
	node->collide(nextNode);
	if (nextNode->id * 2 < nodesNum) collide(node, nodes[nextNode->id * 2]);
	if (nextNode->id * 2 + 1 < nodesNum) collide(node, nodes[nextNode->id * 2 + 1]);
}