#include "GenMain.h"

GenMain::GenMain(
	size_t sizePopulation,
	size_t maxCountPopulation,
	size_t elitismCount,
	double target,
	Generator generator,
	Improver improver,
	Meter& meter,
	Simplifier simplifier
) : sizePopulation(sizePopulation),
maxCountPopulation(maxCountPopulation),
elitismCount(elitismCount),
target(target),
generator(move(generator)),
improver(move(improver)),
meter(meter),
simplifier(move(simplifier))
{
	if (sizePopulation<1) throw logic_error("");
};

Population GenMain::createPopulation() {
	Population population(sizePopulation);
	for (size_t i = 0; i < sizePopulation; i++) {
		unique_ptr<Node> tree = generator.generate();
		population.addIndividual(tree, meter.measure(t));
	}

}

unique_ptr<Node> GenMain::getFuncion() {
	
}