#include "Population.h"
#include <iostream>

Population::Population(
	size_t sizePopulation,
	Meter& meter,
	Generator& generator
) : sizePopulation(sizePopulation),
meter(meter),
repairer(generator)
{
	individuals.reserve(sizePopulation);
}

void Population::addIndividual(
	unique_ptr<Node> node
) {
	double measure = meter.measure(*node.get());
	if (measure == numeric_limits<double>::max()) {
		repairer.repair(node);
		measure = meter.measure(*node.get());
	}
	individuals.emplace_back(
		move(node),
		measure
	);
	if (measureBest > measure) {
		idBest = individuals.size()-1;
		measureBest = measure;
	}
}

void Population::addIndividual(
	unique_ptr<Node> node,
	double measure
) {
	individuals.emplace_back(
		move(node),
		measure
	);
	if (measureBest > measure) {
		idBest = individuals.size() - 1;
		measureBest = measure;
	}
}

size_t Population::getSize() const{
	return individuals.size();
}

const Individual& Population::getIndividual(size_t index) const {
	return individuals[index];
}

const vector<Individual>& Population::getIndividuals() const {
	return individuals;
}

void Population::clear() {
	individuals.clear();
	idBest = 0;
	measureBest = +INFINITY;
}

size_t Population::getIdBest() const {
	return idBest;
}

double Population::getMeasureBest() const {
	return measureBest;
}

void Population::replaceWorst(Individual ind) {
	size_t idWorst = 0;
	for (size_t i = 0; i < sizePopulation; i++)
		if (individuals[i].measure > individuals[idWorst].measure)
			idWorst = i;
	individuals[idWorst] = move(ind);
	if (measureBest > ind.measure) {
		measureBest = ind.measure;
		idBest = idWorst;
	}
}

Population::Population(Population&& population):
	individuals(move(population.individuals)),
	sizePopulation(population.sizePopulation),
	meter(population.meter),
	idBest(population.idBest),
	measureBest(population.measureBest),
	repairer(move(population.repairer)) {}