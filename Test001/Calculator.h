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

	CalcResult tryCalculate(
		Node& node,
		const vector<int>& variables
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

};