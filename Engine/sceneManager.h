#pragma once

#include "application.h"

class SceneManager {
public:

	static std::shared_ptr<Application> application;

	static bool loadScene(std::string sceneName) {
		return application->activateScreen(sceneName);
	}
};