#include "app.h"
#include <Engine/screen.h>
#include "menuScreen.h"
#include "gameScreen.h"
#include <Engine/Mesh/triangle.h>

std::shared_ptr<App> app = std::make_shared<App>();
std::shared_ptr<Application> Screen::application = app;

App::App(){
	srand(time(NULL));
}

App::~App(){

}

void App::start() {
	loadResources();

	addScreen("menu", std::make_shared<MenuScreen>());
	addScreen("game", std::make_shared<GameScreen>());
	
	activateScreen("menu");
}

void App::loadResources()
{
	Global::graphics.addMaterial("button", "Resources/Images/button.png");

	Global::graphics.addMaterial("monokuma", "Resources/Images/monokuma.jpg");
	Global::graphics.addMaterial("monomi", "Resources/Images/monomi.jpg");
	Global::graphics.addMaterial("wall", "Resources/Images/wall.jpg");
	Global::graphics.addMaterial("wallNormal", "Resources/Images/wall_normal.jpg", GL_TEXTURE1, 1.f);
	Global::graphics.addMaterial("ground", "Resources/Images/ground.jpg");
	Global::graphics.addMaterial("groundNormal", "Resources/Images/ground_normal.jpg", GL_TEXTURE1, 1.f);
	Global::graphics.addMaterial("connector", "Resources/Images/connector.jpg");
	Global::graphics.addMaterial("connectorNormal", "Resources/Images/connector_normal.jpg", GL_TEXTURE1, 1.f);
	Global::graphics.addMaterial("platform", "Resources/Images/platform.jpg");
	Global::graphics.addMaterial("platformNormal", "Resources/Images/platform_normal.jpg", GL_TEXTURE1, 1.f);

	Global::graphics.addShape("coin", "./Resources/Meshes/coin.obj");
	Global::graphics.addShape("health", "./Resources/Meshes/health.obj");
	Global::graphics.addShape("bullet", "./Resources/Meshes/ninja_star.obj");
}