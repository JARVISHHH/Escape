#pragma once

#include "Engine/screen.h"
#include "app.h"

#include "Graphics/global.h"

#include <GLFW/glfw3.h>

#include "Engine/Game/gameWorld.h"
#include "Engine/Game/gameSystems/drawSystem.h"
#include <Engine/Game/gameSystems/collisionSystem.h>
#include <Engine/Game/gameSystems/physicsSystem.h>

class GameScreen : public Screen {

public:
	GameScreen();

    void init();

    void update(double seconds) override;
    void draw() override;

    std::shared_ptr<GameObject> createCharacter();
    std::vector<std::shared_ptr<GameObject>> createGrounds();

    std::shared_ptr<GameObject> createFalling(glm::vec3 pos);
    void fallFalling();

    void addScore();

    void checkResult();

private:
    std::shared_ptr<DrawSystem> drawSystem;
    std::shared_ptr<PhysicsSystem> physicsSystem;
    std::shared_ptr<CollisionSystem> collisionSystem;

    int score = 0;
    std::string result = "";
    float time = 0;
    float maxTime = 20;
    int fallingNumber = 0;
};