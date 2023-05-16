#pragma once

#include <Engine/screen.h>

class InstructionScreen : public Screen {
public:
	void init() override;
	void draw() override;
};