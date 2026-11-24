#pragma once
#include "Individual.h"
#include "Meter.h"
#include <vector>
#include <stdexcept>
#include "Repairer.h"

class Population {
private:
	vector<Individual> individuals;
	size_t sizePopulation;
	const Meter& meter;

	size_t idBest=0;
	double measureBest=+INFINITY;

	Repairer repairer;

public:
	Population(
		size_t sizePopulation,
		Meter& meter,
		Generator& generator
	);

	void addIndividual(
		unique_ptr<Node> node
	);

	void addIndividual(
		unique_ptr<Node> node,
		double measure
	);

	size_t getSize() const;

	const Individual& getIndividual(size_t index) const;

	const vector<Individual>& getIndividuals() const;

	void clear();

	size_t getIdBest() const;

	double getMeasureBest() const;

	void replaceWorst(Individual ind);

	Population& operator=(Population&& population);
	
	Population(Population&& population);
};