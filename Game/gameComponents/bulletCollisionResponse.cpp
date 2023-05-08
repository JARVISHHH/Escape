#include "bulletCollisionResponse.h"
#include <Game/gameComponents/movingComponent.h>

BulletCollisionResponse::BulletCollisionResponse()
	:CollisionResponseComponent()
{
}

BulletCollisionResponse::BulletCollisionResponse(bool isStatic)
	:CollisionResponseComponent(isStatic)
{
}

void BulletCollisionResponse::responseCollision(std::shared_ptr<CollisionResponseComponent> otherComponent, glm::vec3 mtv)
{
	getGameObject()->setActiveStatus(false);

	for(int i = 0; i < 50; i++)
		getGameObject()->getGameWorld()->getGameSystem<ParticleSystem>("particle")->emit(createBloodParticle());
}

void BulletCollisionResponse::responseCollision(std::vector<std::shared_ptr<CollisionInfo>>& collisions, glm::vec3 curPos)
{
	if (collisions.size() <= 0) return;
	getGameObject()->setActiveStatus(false);
	for (int i = 0; i < 50; i++)
		getGameObject()->getGameWorld()->getGameSystem<ParticleSystem>("particle")->emit(createGreenParticle());
}

Particle BulletCollisionResponse::createBloodParticle()
{
	Particle particle;

	particle.position = getGameObject()->getComponent<TransformComponent>("transform")->getModelTransform()->getPos();
	
	particle.velocityBegin = -getGameObject()->getComponent<MovingComponent>("moving")->getDirection() * 2.0f;
	particle.velocityEnd = glm::vec3(0, 0, 0);

	particle.colorBegin = glm::vec4(1.0, 0, 0, 1.0);
	particle.colorEnd = glm::vec4(1.0, 1.0, 0, 1.0);

	particle.sizeBegin = glm::vec3(0.005, 0.005, 0.005);
	particle.sizeEnd = glm::vec3(0.001, 0.001, 0.001);

	particle.lifeTime = 1;

	return particle;
}

Particle BulletCollisionResponse::createGreenParticle()
{
	Particle particle;

	particle.position = getGameObject()->getComponent<TransformComponent>("transform")->getModelTransform()->getPos();

	particle.velocityBegin = -getGameObject()->getComponent<MovingComponent>("moving")->getDirection() * 2.0f;
	particle.velocityEnd = glm::vec3(0, 0, 0);

	particle.colorBegin = glm::vec4(0.5, 1.0, 0, 1.0);
	particle.colorEnd = glm::vec4(0.5, 0.5, 0, 1.0);

	particle.sizeBegin = glm::vec3(0.005, 0.005, 0.005);
	particle.sizeEnd = glm::vec3(0.001, 0.001, 0.001);

	particle.lifeTime = 1;

	return particle;
}