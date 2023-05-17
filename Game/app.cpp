#include "app.h"
#include <Engine/screen.h>
#include <Engine/sceneManager.h>
#include "menuScreen.h"
#include "gameScreen.h"
#include "instructionScreen.h"
#include <Engine/Mesh/triangle.h>

std::shared_ptr<App> app = std::make_shared<App>();
std::shared_ptr<Application> Screen::application = app;
std::shared_ptr<Application> SceneManager::application = app;

App::App(){
	srand(time(NULL));
}

App::~App(){

}

void App::start() {
	loadResources();

	addScreen("menu", std::make_shared<MenuScreen>());
	addScreen("instructions", std::make_shared<InstructionScreen>());
	addScreen("game", std::make_shared<GameScreen>());
	
	activateScreen("menu");
}

void App::loadResources()
{
	Global::graphics.addMaterial("title", "Resources/Images/title.png");
	Global::graphics.addMaterial("instructions", "Resources/Images/instructions.png");

	Global::graphics.addMaterial("button", "Resources/Images/button.png");
	Global::graphics.addMaterial("shoot", "Resources/Images/shoot.png");

	Global::graphics.addMaterial("hp", "Resources/Images/hp.png");
	Global::graphics.addMaterial("hp_red", "Resources/Images/hp_red.png");
	Global::graphics.addMaterial("time", "Resources/Images/time.png");
	Global::graphics.addMaterial("time_blue", "Resources/Images/time_blue.png");

	Global::graphics.addMaterial("die", "Resources/Images/die.png");
	Global::graphics.addMaterial("win", "Resources/Images/win.png");

	Global::graphics.addMaterial("trashcan", "Resources/Images/TrashCan.png");
	Global::graphics.addMaterial("trashcanNormal", "Resources/Images/TrashCanNormal.png", GL_TEXTURE1, 1.f);
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
	Global::graphics.addShape("character", "./Resources/Meshes/character.obj");
	Global::graphics.addShape("enemy", "./Resources/Meshes/enemy.obj");

	Screen::addEnvironmentMesh("wall", "./Resources/Meshes/wall.obj", true, 10);
	Screen::addEnvironmentMesh("ground", "./Resources/Meshes/ground.obj", true, 10);
	Screen::addEnvironmentMesh("plane", "./Resources/Meshes/plane.obj", true, 2);
	Screen::addEnvironmentMesh("box", "./Resources/Meshes/box.obj", true, 10);
	Screen::addEnvironmentMesh("spike", "./Resources/Meshes/spike.obj");
	Screen::addEnvironmentMesh("test", "./Resources/Meshes/test.obj");
}