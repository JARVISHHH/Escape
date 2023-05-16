#pragma once

#include "Engine/application.h"

class App : public Application
{
public:
	App();
	~App();

	void start() override;
	void loadResources();

private:

};