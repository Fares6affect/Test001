#include "Population.h"

Population::Population(size_t capacity) : capacity(capacity) {
	if (capacity < 1)
		throw logic_error("Размер популяции меньше нуля");
	individuals.reserve(capacity);
}

void Population::addIndividual(
	unique_ptr<Node> tree,
	double measure
) {
	if (individuals.size() >= capacity)
		throw logic_error("");
	individuals.emplace_back(
		move(tree),
		measure
	);
}

size_t Population::getSize() {
	return individuals.size();
}

const Individual& Population::getIndividual(size_t index) const {
	return individuals[index];
}

void Population::clear() {
	individuals.clear();
}

vector<double> Population::getMeasure() const {
	vector<double> measures;
	measures.reserve(individuals.size());
	for (const Individual& individual : individuals)
		measures.push_back(individual.measure);
	return measures;
}