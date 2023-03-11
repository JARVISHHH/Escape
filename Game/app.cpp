#include "app.h"
#include "menuScreen.h"
#include "gameScreen.h"
#include <Engine/Game/Mesh/triangle.h>

std::shared_ptr<App> app = std::make_shared<App>();

App::App(){
	srand(time(NULL));
}

App::~App(){

}

void App::start() {
	auto temp = Global::graphics.addShape("ceiling", "./Resources/Meshes/ceiling.obj");
	for (int i = 0; i < temp.size(); i += 3) {
		auto t = Triangle(temp[i], temp[i + 1], temp[i + 2]);
	}

	addScreen("menu", std::make_shared<MenuScreen>());
	addScreen("game", std::make_shared<GameScreen>());
	
	activateScreen("menu");
}