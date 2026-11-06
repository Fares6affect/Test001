#include "GenMain.h"

GenMain::GenMain()
{
	if (sizePopulation<1) throw logic_error("");

	meter = Meter();

	generator = new Generator(
		meter.getMaxDepth(),
		meter.getCountVariables()
	);

	improver = new Improver(
		meter.getMaxDepth(),
		meter.getSizePopulation(),
		meter.getCountVariables(),
		*generator
	);

	simplifier = Simplifier();
};

/*Population GenMain::createPopulation() {
	Population population(sizePopulation,meter);
	for (size_t i = 0; i < sizePopulation; i++) {
		unique_ptr<Node> tree = generator.generate();
		population.addIndividual(tree, meter.measure(tree));
	}
}*/

unique_ptr<Node> GenMain::getFuncion() {
	unique_ptr<Node> a;
	return a;
}