#include "app.h"
#include "menuScreen.h"
#include "gameScreen.h"
#include <Engine/Mesh/triangle.h>

std::shared_ptr<App> app = std::make_shared<App>();

App::App(){
	srand(time(NULL));
}

App::~App(){

}

void App::start() {

	addScreen("menu", std::make_shared<MenuScreen>());
	addScreen("game", std::make_shared<GameScreen>());
	
	activateScreen("menu");
}