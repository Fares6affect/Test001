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

unique_ptr<Node> GenMain::getFuncion() {

}