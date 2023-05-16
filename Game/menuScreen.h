#pragma once

#include "app.h"

#include "Graphics/global.h"

#include <GLFW/glfw3.h>

#include <Engine/screen.h>

class MenuScreen : public Screen {

public:
	void init() override;
	void draw() override;
	//void keyEvent(int key, int action) override;

	static void activeGame();
};