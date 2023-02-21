#pragma once

#include "Engine/screen.h"
#include "app.h"

#include "Graphics/global.h"

#include <GLFW/glfw3.h>

#include "Engine/Game/gameWorld.h"

class GameScreen : public Screen {

public:
	GameScreen();

    void init();

    void update(double seconds) override;
    void draw() override;

    std::shared_ptr<GameObject> createCharacter();
    std::vector<std::shared_ptr<GameObject>> createGrounds();

    std::shared_ptr<GameObject> createFalling();
};