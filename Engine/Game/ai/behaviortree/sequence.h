#pragma once

#include "composite.h"

class Sequence : public Composite {
public:
	BTStatus update(double seconds) override;
	void reset() override;

private:


};