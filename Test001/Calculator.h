#pragma once
#include "Node.h"
#include <vector>
#include <string>
#include <stdexcept>
#include "Element.h"
#include <cmath>
#include "SimpleElement.h"

class Calculator {
public:

	double calculate(
		const Node& node,
		const vector<int>& variables
	) const;

	string getFormula(
		const Node& node
	) const;

	vector<SimpleElement> getElements(
		const Node&
	) const;

	string getOptimizeFormula(
		const Node& node
	) const;

	void repair(Node& parent, Node& node);
};