#pragma once
#include "Individual.h"
#include "Meter.h"
#include <vector>
#include <stdexcept>

class Population {
private:
	vector<Individual> individuals;
	size_t sizePopulation;
	const Meter& meter;

	size_t idBest=0;
	double measureBest=+INFINITY;
	vector<double> measures;

public:
	Population(
		size_t sizePopulation,
		Meter& meter
	);

	void addIndividual(
		unique_ptr<Node> node
	);

	size_t getSize() const;

	const Individual& getIndividual(size_t index) const;

	void clear();

	const vector<double>& getMeasures() const;

	size_t getIdBest() const;

	double getMeasureBest() const;

	Population& operator=(Population&& population);
	
	Population(Population&& population);
};