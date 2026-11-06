#pragma once
#include "Generator.h"
#include "Improver.h"
#include "Meter.h"
#include "Simplifier.h"
#include "Population.h"
#include <utility>

class GenMain {
private:
	size_t sizePopulation;
	size_t maxCountPopulation;
	size_t elitismCount;
	double target;

	Meter meter;
	Generator* generator;
	Improver* improver;
	Simplifier simplifier;

	Population createPopulation();

public:
	GenMain();

	unique_ptr<Node> getFuncion();
};