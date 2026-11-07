#pragma once
#include "Data.h"
#include "Calculator.h"

class Meter {
private:
	const vector<Data>& data;
	double fine;
	Calculator calculator;

public:
	Meter(
		const vector<Data>& data,
		double fine
	);

	double measure(const Node& node) const;
};