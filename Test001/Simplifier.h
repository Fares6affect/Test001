#pragma once
#include "Calculator.h"

class Simplifier {
	Calculator calculator;
public:
	void simplify(unique_ptr<Node>& node) const;
};