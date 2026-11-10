#include "Island.h"
#include <iostream>

Island::Island(
	size_t sizePopulation,
	size_t sizeSelection,
	size_t elitismCount,

	size_t maxDepth,
	size_t countVariables,
	Meter& meter
):sizePopulation(sizePopulation),
	elitismCount(elitismCount),
	meter(meter)
{
	generator = make_unique<Generator>(
		maxDepth,
		countVariables
	);

	improver = make_unique<Improver>(
		maxDepth,
		sizeSelection,
		countVariables,
		*generator
	);
}

void Island::createPopulation() {
	population = make_unique<Population>(sizePopulation, meter);
	for (size_t i = 0; i < sizePopulation; i++) {
		unique_ptr<Node> node = generator.get()->generate();
		simplifier.simplify(node);
		population.get()->addIndividual(move(node));
	}
}

unique_ptr<Node> Island::getBestNode() const {
	return population.get()->getIndividual(population.get()->getIdBest()).node.get()->clone();
}

vector<unique_ptr<Node>> Island::getBestNodes(size_t count) const {
	vector<unique_ptr<Node>> result;
	result.reserve(count);

	vector<bool> selected(population->getSize(), false);
	const vector<double>& measures = population->getMeasures();

	for (size_t i = 0; i < count; i++) {
		size_t idBest = population->getSize();
		double measureBest = INFINITY;

		for (size_t j = 0; j < population->getSize(); j++) {
			if (!selected[j] && measures[j] < measureBest) {
				measureBest = measures[j];
				idBest = j;
			}
		}

		selected[idBest] = true;

		result.push_back(
			population->getIndividual(idBest).node->clone()
		);
	}

	return result;
}

double Island::getBestMeasure() const {
	return population.get()->getMeasureBest();
}

void Island::migration(unique_ptr<Node> migrant) {
	population.get()->replaceWorst(move(migrant));
}

void Island::migrations(vector<unique_ptr<Node>> migrants) {
	for (unique_ptr <Node>& migrant : migrants)
		population->replaceWorst(move(migrant));
}

void Island::nextPopulation() {
	Population newPopulation(sizePopulation, meter);
	vector<bool> selectedNodes(sizePopulation, false);
	const vector<double>& measures = population.get()->getMeasures();
	for (size_t i = 0; i < elitismCount; i++) {
		size_t idBestNode = sizePopulation;
		double measureBestNode = +INFINITY;
		for (size_t j = 0; j < sizePopulation; j++)
			if (!selectedNodes[j] && measures[j] < measureBestNode) {
				measureBestNode = measures[j];
				idBestNode = j;
			}
		selectedNodes[idBestNode] = true;
		const Individual& individual= population.get()->getIndividual(idBestNode);
		newPopulation.addIndividual(
			individual.node.get()->clone(),
			individual.measure
			);
	}
	while (newPopulation.getSize() < sizePopulation) {
		size_t parent1 = improver.get()->selection(measures);
		size_t parent2 = improver.get()->selection(measures);
		unique_ptr<Node> son = improver.get()->improve(
			*population.get()->getIndividual(parent1).node,
			*population.get()->getIndividual(parent2).node
		);
		simplifier.simplify(son);
		newPopulation.addIndividual(move(son));
	}
	population = make_unique<Population>(move(newPopulation));
}