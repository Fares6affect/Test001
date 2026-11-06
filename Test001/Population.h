#pragma once
#include "Individual.h"
#include "Meter.h"
#include <vector>
#include <stdexcept>

class Population {
private:
	vector<Individual> individuals;
	size_t capacity;
	Meter meter;
public:
	Population(
		size_t capacity,
		Meter& meter
	);

	void addIndividual(
		unique_ptr<Node> tree
	);

	size_t getSize();

	const Individual& getIndividual(size_t index) const;

	void clear();

	vector<double> getMeasure() const;
};