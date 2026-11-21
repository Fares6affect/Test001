#include "Population.h"
#include <iostream>

Population::Population(
	size_t sizePopulation,
	Meter& meter
	) : sizePopulation(sizePopulation),
	meter(meter)
{
	individuals.reserve(sizePopulation);
}

void Population::addIndividual(
	unique_ptr<Node> node
) {
	double measure = meter.measure(*node.get());
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

void Population::clear() {
	individuals.clear();
	idBest = 0;
	measureBest = +INFINITY;
}

const vector<double>& Population::getMeasures() const {
	vector<double> res;
	res.reserve(individuals.size());
	for (size_t i = 0; i < sizePopulation; i++)
		res.push_back(individuals[i].measure);
	return res;
}

size_t Population::getIdBest() const {
	return idBest;
}

double Population::getMeasureBest() const {
	return measureBest;
}

void Population::replaceWorst(unique_ptr<Node> node) {
	double measure = meter.measure(*node);
	size_t idWorst = 0;
	for (size_t i = 0; i < sizePopulation; i++)
		if (individuals[i].measure > individuals[idWorst].measure)
			idWorst = i;
	individuals[idWorst] = Individual(
		move(node),
		measure
	);
	individuals[idWorst].measure = measure;
	if (measureBest > measure) {
		measureBest = measure;
		idBest = idWorst;
	}
}

Population::Population(Population&& population):
	individuals(move(population.individuals)),
	sizePopulation(population.sizePopulation),
	meter(population.meter),
	idBest(population.idBest),
	measureBest(population.measureBest){}