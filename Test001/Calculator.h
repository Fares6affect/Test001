#pragma once
#include "Node.h"
#include <vector>
#include <string>
#include <stdexcept>
#include "Element.h"
#include <cmath>
#include <map>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <limits>
#include "SimpleElement.h"
#include "CalcResult.h"

class Calculator {
public:

	double calculate(
		const Node& node,
		const vector<int>& variables
	) const;

	CalcResult tryCalculate(
		const Node& node,
		const vector<int>& variables
	) const;

	string getFormula(
		const Node& node
	) const;

	vector<SimpleElement> simplifyElements(
		const vector<SimpleElement>& elements
	) const;
	
	vector<SimpleElement> getElements(
		const Node&
	) const;

	string getOptimizeFormula(
		const Node& node
	) const;

	void repair(Node& parent, Node& node);
};