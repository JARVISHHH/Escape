#include "particleSystem.h"
#include <Engine/random.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>

ParticleSystem::ParticleSystem()
	:GameSystem("particle")
{
	particlePool.resize(1000);
}

void ParticleSystem::update(double seconds)
{
	for (auto& particle : particlePool) {
		if (!particle.active) continue;

		if (particle.lifeRemaining <= 0.0f)
		{
			particle.active = false;
			continue;
		}

		particle.lifeRemaining -= seconds;

		float life = particle.lifeRemaining / particle.lifeTime;
		auto velocity = glm::lerp(particle.velocityEnd, particle.velocityBegin, life);
		particle.position += velocity * (float)seconds;
	}
}

void ParticleSystem::draw()
{
	Global::graphics.bindShader("particle");
	Global::graphics.setCameraData(getGameWorld()->getCamera());

	for (auto& particle : particlePool) {
		if (!particle.active)
			continue;

		float life = particle.lifeRemaining / particle.lifeTime;

		glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);
		//color = particle.colorEnd;
		color.a = color.a * life;

		glm::vec3 size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);
	
		auto modelTransform = std::make_shared<ModelTransform>();
		modelTransform->scale(2.0f * size);
		modelTransform->translate(particle.position);
		Global::graphics.setUniform4fv("u_Color", color);
		Global::graphics.drawShape(Global::graphics.getShape("sphere"), modelTransform, nullptr);
	
		//std::cout << "draw" << std::endl;
	}
}

void ParticleSystem::emit(const Particle& particleInfo) {
	Particle& particle = particlePool[poolIndex];

	// Position
	particle.position = particleInfo.position;
	
	// Velocity
	particle.velocityBegin = particleInfo.velocityBegin;
	particle.velocityEnd = particle.velocityEnd;
	for (int i = 0; i < 3; i++) {
		particle.velocityBegin[i] += particleInfo.velocityBegin[i] * (Random::Float() - 0.5f);
	}

	// Color
	particle.colorBegin = particleInfo.colorBegin;
	particle.colorEnd = particleInfo.colorEnd;

	// Size
	particle.sizeBegin = particleInfo.sizeBegin;
	particle.sizeEnd = particleInfo.sizeEnd;

	// Life time
	particle.lifeTime = particleInfo.lifeTime;
	particle.lifeRemaining = particleInfo.lifeTime;

	poolIndex = --poolIndex % particlePool.size();

	particle.active = true;
}