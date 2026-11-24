#include "Island.h"
#include <iostream>

Island::Island(
	size_t sizePopulation,
	size_t sizeSelection,
	size_t elitismCount,

	size_t maxDepth,
	size_t countVariables,
	Meter& meter,
	IslandStrategy strategy,

	double target
):sizePopulation(sizePopulation),
	elitismCount(elitismCount),
	meter(meter),
	constOptimizer(meter,10,1,0.1,target,3,1)
{
	generator = make_unique<Generator>(
		maxDepth,
		countVariables,
		strategy
	);

	improver = make_unique<Improver>(
		maxDepth,
		sizeSelection,
		countVariables,
		*generator,
		strategy,
		target
	);
}

void Island::createPopulation() {
	cout << endl << endl;
	population = make_unique<Population>(sizePopulation, meter, *generator);
	for (size_t i = 0; i < sizePopulation; i++) {
		unique_ptr<Node> node = generator.get()->generate();
		simplifier.simplify(node);
		constOptimizer.optimize(*node);
		//
		Calculator calculator;
		if (node.get()->el == Element::Exp) cout<<calculator.getOptimizeFormula(*node)<<endl;
		//
		population.get()->addIndividual(move(node));
	}
	improver.get()->updateProbability(getBestMeasure());
}

unique_ptr<Node> Island::getBestNode() const {
	return population.get()->getIndividual(population.get()->getIdBest()).node.get()->clone();
}

vector<Individual> Island::getBestIndividuals(size_t count) const {
	vector<Individual> result;
	result.reserve(count);
	vector<bool> selected(population->getSize(), false);
	const vector<Individual>& individuals = population->getIndividuals();
	for (size_t i = 0; i < count; i++) {
		size_t idBest = population->getSize();
		double measureBest = INFINITY;
		for (size_t j = 0; j < population->getSize(); j++) {
			if (!selected[j] && individuals[j].measure < measureBest) {
				measureBest = individuals[j].measure;
				idBest = j;
			}
		}
		selected[idBest] = true;
		result.emplace_back(
			individuals[idBest].node.get()->clone(),
			individuals[idBest].measure
		);
	}
	return result;
}

double Island::getBestMeasure() const {
	return population.get()->getMeasureBest();
}

void Island::migrations(vector<Individual> migrants) {
	for (Individual& migrant : migrants)
		population->replaceWorst(move(migrant));
}

void Island::nextPopulation() {
	Population newPopulation(sizePopulation, meter, *generator);
	const vector<Individual>& individuals = population.get()->getIndividuals();
	vector<Individual> elite = getBestIndividuals(elitismCount);
	for (Individual& ind : elite)
		newPopulation.addIndividual(move(ind.node),ind.measure);
	while (newPopulation.getSize() < sizePopulation) {
		size_t parent1 = improver.get()->selection(individuals);
		size_t parent2 = improver.get()->selection(individuals);
		unique_ptr<Node> son = improver.get()->improve(
			*population.get()->getIndividual(parent1).node,
			*population.get()->getIndividual(parent2).node
		);
		simplifier.simplify(son);
		constOptimizer.optimize(*son);
		//
		/*Calculator calculator;
		if (son.get()->el == Element::Exp) cout<<calculator.getOptimizeFormula(*son)<<endl;*/
		//
		newPopulation.addIndividual(move(son));
	}
	population = make_unique<Population>(move(newPopulation));
	improver.get()->updateProbability(getBestMeasure());
}