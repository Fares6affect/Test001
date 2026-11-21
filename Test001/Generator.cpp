#include "Generator.h"

Generator::Generator(
	size_t maxDepth,
	size_t countVariables,
	IslandStrategy strategy
):rand(random_device{}()),
	maxDepth(maxDepth),
	countVariables(countVariables),
	config(getGeneratorConfig(strategy)){}

unique_ptr<Node> Generator::generateTerminal() {
	uniform_int_distribution<> constOrVar(0, 1);
	if (constOrVar(rand)) {
		normal_distribution<> exponentDist(0.0, 2.0);
		uniform_int_distribution<> signDist(0,1);
		return Node::makeConst((signDist(rand)*2-1)*pow(10.0,exponentDist(rand)));
	}
	uniform_int_distribution<> varDist(0, countVariables - 1);
	return Node::makeVar(
		varDist(rand)
	);
}

unique_ptr<Node> Generator::generateUnarOper(size_t depth) {
	const array<Element, 6> operators = {
		Element::Exp,
		Element::Log,
		Element::Neg,
		Element::Conv,
		Element::Sqrt,
		Element::Abs
	};
	discrete_distribution<size_t> unarDist(
		config.unarOperProb.begin(),
		config.unarOperProb.end()
	);
	unique_ptr<Node> son;
	do {
		son = generate(depth + 1);
	} while (son.get()->el == Element::Const);
	return Node::makeUnarOper(
		operators[unarDist(rand)],
		move(son)
	);
}

unique_ptr<Node> Generator::generateBinOper(size_t depth) {
	const array<Element, 2> operators = {
		Element::Add,
		Element::Mul,
	};
	discrete_distribution<size_t> binDist(
		config.binOperProb.begin(),
		config.binOperProb.end()
	);
	unique_ptr<Node> son1 = generate(depth + 1);
	unique_ptr<Node> son2 = generate(depth + 1);
	while (son1.get()->el == Element::Const && son2.get()->el == Element::Const) {
		son2 = generate(depth + 1);
	}
	return Node::makeBinOper(
		operators[binDist(rand)],
		move(son1),
		move(son2)
	);
}

unique_ptr<Node> Generator::generate(size_t depth) {
	if (depth >= maxDepth)
		return generateTerminal();
	double progress = static_cast<double>(depth - 1) / (maxDepth - 2);
	double termProb = config.termProb + (config.maxTermProb-config.termProb) * progress;
	double operProb = config.unarProb + config.binProb;
	double unarProb = (100.0 - termProb) * config.unarProb / operProb;
	double binProb = (100.0 - termProb) * config.binProb / operProb;

	array<double, 3> probs = {
		termProb,
		unarProb,
		binProb
	};
	discrete_distribution<size_t> dist = discrete_distribution<size_t>(probs.begin(), probs.end());

	switch (dist(rand)) {
	case 0: return generateTerminal();
	case 1: return generateUnarOper(depth);
	case 2: return generateBinOper(depth);
	}
};