#pragma once

#include "Engine/Game/components/physicsComponent.h"

class FallingPhysics : public PhysicsComponent {
public:
	FallingPhysics();

	void update(double seconds) override;
};
