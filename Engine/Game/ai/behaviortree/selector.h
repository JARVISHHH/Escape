#pragma once

#include "composite.h"

class Selector : public Composite {
public:
	BTStatus update(double seconds) override;
	void reset() override;

private:


};