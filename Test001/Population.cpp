#include "Population.h"

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
	measures.push_back(measure);
	if (measureBest > measure) {
		idBest = measures.size()-1;
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
	measures.clear();
	idBest = 0;
	measureBest = +INFINITY;
}

const vector<double>& Population::getMeasures() const {
	return measures;
}

size_t Population::getIdBest() const {
	return idBest;
}

double Population::getMeasureBest() const {
	return measureBest;
}

Population& Population::operator=(Population&& population) {
	individuals = move(population.individuals);
	idBest = population.idBest;
	measureBest = population.measureBest;
	measures = move(population.measures);
	return *this;
};

Population::Population(Population&& population):
	individuals(move(population.individuals)),
	sizePopulation(population.sizePopulation),
	meter(population.meter),
	idBest(population.idBest),
	measureBest(population.measureBest),
	measures(move(population.measures)){}