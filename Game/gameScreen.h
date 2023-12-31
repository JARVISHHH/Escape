#pragma once

#include "Engine/screen.h"
#include "app.h"

#include "Graphics/global.h"

#include <GLFW/glfw3.h>

#include "Engine/Game/gameWorld.h"
#include "Engine/Game/gameSystems/drawSystem.h"
#include <Engine/Game/gameSystems/collisionSystem.h>
#include <Engine/Game/gameSystems/physicsSystem.h>
#include <Engine/Game/ai/navmesh/navmesh.h>

#include <Engine/screen.h>

#include "ui.h"

class GameScreen : public Screen {

public:
    void init() override;
    void activateAction() override;

    void update(double seconds) override;
    void draw() override;

private:
    std::shared_ptr<DrawSystem> drawSystem;
    std::shared_ptr<PhysicsSystem> physicsSystem;
    std::shared_ptr<CollisionSystem> collisionSystem;

    std::shared_ptr<Image> hpImage;
    std::shared_ptr<Image> timeImage;
    std::shared_ptr<Image> resultImage;
    std::shared_ptr<Image> scoreImage;
};