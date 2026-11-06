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

	Generator generator;
	Improver improver;
	Meter& meter;
	Simplifier simplifier;

	Population createPopulation();

public:
	GenMain(
		size_t sizePopulation,
		size_t maxCountPopulation,
		size_t elitismCount,
		double target,
		Generator generator,
		Improver improver,
		Meter& meter,
		Simplifier simplifier
	);

	unique_ptr<Node> getFuncion();
};