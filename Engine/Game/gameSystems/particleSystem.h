#pragma once

#include "Engine/Game/gameSystem.h"
#include <vector>
#include <GL/glew.h>

struct Particle {
	glm::vec3 position;

	glm::vec3 velocityBegin;
	glm::vec3 velocityEnd;

	glm::vec4 colorBegin;
	glm::vec4 colorEnd;

	glm::vec3 sizeBegin;
	glm::vec3 sizeEnd;

	float lifeTime;
	float lifeRemaining;

	bool active = false;
};

class ParticleSystem: public GameSystem {
public:
	ParticleSystem();

	void update(double seconds);
	void draw();
	void emit(const Particle& particleInfo);

private:
	std::vector<Particle> particlePool;
	int poolIndex = 999;
};