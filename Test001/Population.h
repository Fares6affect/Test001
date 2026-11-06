#pragma once
#include "Individual.h"
#include <vector>
#include <stdexcept>

class Population {
private:
	vector<Individual> individuals;
	size_t capacity;
public:
	Population(size_t capacity);

	void addIndividual(
		unique_ptr<Node> tree,
		double measure
	);

	size_t getSize();

	const Individual& getIndividual(size_t index) const;

	void clear();

	vector<double> getMeasure() const;
};