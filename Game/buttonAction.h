#pragma once

#include <Engine/sceneManager.h>

class ButtonAction {
public:
	static void loadTitle() {
		SceneManager::loadScene("menu");
	}

	static void loadInstruction() {
		SceneManager::loadScene("instructions");
	}

	static void loadGame() {
		SceneManager::loadScene("game");
	}
};