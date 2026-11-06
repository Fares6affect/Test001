#pragma once
#include "Calculator.h"

class Simplifier {
	Calculator calculator;
	vector<int> variables;
public:
	void simplify(unique_ptr<Node>& node) const;
};