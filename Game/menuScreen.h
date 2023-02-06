#pragma once

#include "Engine/application.h"
#include "app.h"

#include "Graphics/global.h"

#include <GLFW/glfw3.h>


class MenuScreen : public Screen {

public:
	MenuScreen();
	~MenuScreen();

	void update(double seconds) override;
	void draw() override;
	void keyEvent(int key, int action) override;
};