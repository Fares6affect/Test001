#pragma once
#include "Data.h"
#include "Calculator.h"
#include <stdexcept>
#include <fstream>
#include <iostream>


class Meter {
private:
	vector<Data> data;
	double fine = 0.0;
	Calculator calculator;

public:

	Meter();

	double measure(Node& node);

};