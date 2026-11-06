#pragma once
#include "Node.h"
#include <vector>
#include <string>
#include <stdexcept>

class Calculator {
public:

	double calculate(
		const Node& node,
		const vector<int>& variables
	) const;

	string getFormula(
		const Node& node
	) const;

	string getOptimizeFormula(
		const Node& node
	) const;

};