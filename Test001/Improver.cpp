#include "Improver.h"

Improver::Improver(
	size_t maxDepth,
	size_t sizeSelection,
	size_t countVariables,
	Generator& generator,
	double target
) :rand(random_device{}()),
maxDepth(maxDepth),
sizeSelection(sizeSelection),
countVariables(countVariables),
generator(generator),
target(target)
{
	if (maxDepth < 1)
		throw logic_error("Максимальная глубина должна быть >=1");
	if (sizeSelection < 1)
		throw logic_error("Количество родителей должно быть >=1");
}

size_t Improver::selection(
	const vector<double>& measures
) {
	if (measures.empty())
		throw logic_error("Список пуст");
	uniform_real_distribution<> selectionDist(0.0, 1.0);
	uniform_int_distribution<> idParent(0, measures.size() - 1);
	if (selectionDist(rand) < mainSelProb) {
		size_t res = 0; double measure = +INFINITY;
		for (size_t i = 0; i < sizeSelection; i++) {
			size_t id = idParent(rand);
			if (measure > measures[id]) {
				measure = measures[id];
				res = id;
			}
		}
		return res;
	}
	else {
		return idParent(rand);
	}
}

void Improver::usingNodes(
	vector<UsingNode>& nodes,
	unique_ptr<Node>& node,
	size_t depth
) {
	if (!node)
		return;
	nodes.push_back({ &node,depth });
	usingNodes(nodes, node->left, depth + 1);
	usingNodes(nodes, node->right, depth + 1);
}

unique_ptr<Node> Improver::mutateConst(const  Node& parent) {
	unique_ptr<Node> son = parent.clone();
	vector<UsingNode> nodes;
	usingNodes(nodes, son, 1);
	vector<UsingNode> currentNodes;
	for (UsingNode& node : nodes)
		if (node.node->get()->el == Element::Const)
			currentNodes.push_back(node);
	if (currentNodes.empty())
		return son;
	uniform_int_distribution<> nodeDist(0, currentNodes.size() - 1);
	UsingNode node = currentNodes[nodeDist(rand)];
	double oldValue = node.node->get()->value;

	double minStep = 0.1;
	double relativeStep = 0.1;
	double sigma = max(minStep, abs(oldValue) * relativeStep);
	normal_distribution<double> noise(0.0, sigma);
	node.node->get()->value = oldValue + noise(rand);
	return son;
}

unique_ptr<Node> Improver::mutateVar(const  Node& parent) {
	if (countVariables == 1) return mutateConst(parent);
	unique_ptr<Node> son = parent.clone();
	vector<UsingNode> nodes;
	usingNodes(nodes, son, 1);
	vector<UsingNode> currentNodes;
	for (UsingNode& node : nodes)
		if (node.node->get()->el == Element::Var)
			currentNodes.push_back(node);
	if (currentNodes.empty())
		return son;
	uniform_int_distribution<> nodeDist(0, currentNodes.size() - 1);
	UsingNode node = currentNodes[nodeDist(rand)];
	uniform_int_distribution<> varDist(0, countVariables - 1);
	size_t oldVariableIndex = node.node->get()->variableIndex;
	do {
		node.node->get()->variableIndex = varDist(rand);
	} while (node.node->get()->variableIndex == oldVariableIndex);
	return son;
}

unique_ptr<Node> Improver::mutateFunction(const Node& parent) {
	unique_ptr<Node> son = parent.clone();
	vector<UsingNode> nodes;
	usingNodes(nodes, son, 1);
	vector<UsingNode> currentNodes;
	for (UsingNode& node : nodes)
		if (Node::arity(node.node->get()->el) > 0)
			currentNodes.push_back(node);
	if (currentNodes.empty())
		return son;
	uniform_int_distribution<> nodeDist(0, currentNodes.size() - 1);
	UsingNode node = currentNodes[nodeDist(rand)];
	Element nodeEl = node.node->get()->el;
	Element el;
	if (Node::arity(nodeEl) == 1) {
		uniform_int_distribution<> unarOperDist(2, 6);
		do {
			el = static_cast<Element>(unarOperDist(rand));
		} while (nodeEl == el);
	}
	else {
		uniform_int_distribution<> binOperDist(7, 8);
		do {
			el = static_cast<Element>(binOperDist(rand));
		} while (nodeEl == el);
	}
	node.node->get()->el = el;
	return son;
}

unique_ptr<Node> Improver::mutateSubTree(const Node& parent) {
	unique_ptr<Node> son = parent.clone();
	vector<UsingNode> nodes;
	usingNodes(nodes, son, 1);
	if (nodes.empty())
		return son;
	uniform_int_distribution<> nodeDist(0, nodes.size() - 1);
	UsingNode node = nodes[nodeDist(rand)];
	*node.node = move(generator.generate(node.depth));
	return son;
}

unique_ptr<Node> Improver::mutateDelete(const Node& parent) {
	unique_ptr<Node> son = parent.clone();
	vector<UsingNode> nodes;
	usingNodes(nodes, son, 1);
	vector<UsingNode> currentNodes;
	for (UsingNode& node : nodes)
		if (Node::arity(node.node->get()->el) > 0)
			currentNodes.push_back(node);
	if (currentNodes.empty())
		return son;
	uniform_int_distribution<> nodeDist(0, currentNodes.size() - 1);
	size_t id = nodeDist(rand);
	if (Node::arity(currentNodes[id].node->get()->el) == 1)
		*currentNodes[id].node = move(currentNodes[id].node->get()->left);
	else {
		uniform_int_distribution<> sideDist(0, 1);
		if (sideDist(rand))
			*currentNodes[id].node = move(currentNodes[id].node->get()->left);
		else
			*currentNodes[id].node = move(currentNodes[id].node->get()->right);
	}
	return son;
}

unique_ptr<Node> Improver::mutateInsert(const Node& parent) {
	unique_ptr<Node> son = parent.clone();
	vector<UsingNode> nodes;
	usingNodes(nodes, son, 1);
	vector<UsingNode> currentNodes;
	for (UsingNode& node : nodes)
		if (node.depth + node.node->get()->depth() <= maxDepth)
			currentNodes.push_back(node);
	if (currentNodes.empty())
		return son;
	uniform_int_distribution<> nodeDist(0, currentNodes.size() - 1);
	UsingNode node = currentNodes[nodeDist(rand)];
	uniform_int_distribution unarOperDist(2, 6);
	unique_ptr<Node> root = Node::makeUnarOper(
		static_cast<Element>(unarOperDist(rand)),
		move(*node.node)
	);
	*node.node = move(root);
	return son;
}

unique_ptr<Node> Improver::mutateSwap(const Node& parent) {
	unique_ptr<Node> son = parent.clone();
	vector<UsingNode> nodes;
	usingNodes(nodes, son, 1);
	vector<UsingNode> currentNodes;
	for (UsingNode& node : nodes)
		if (Node::arity(node.node->get()->el) == 2)
			currentNodes.push_back(node);
	if (currentNodes.empty())
		return son;
	uniform_int_distribution<> nodeDist(0, currentNodes.size() - 1);
	UsingNode node = currentNodes[nodeDist(rand)];
	node.node->get()->left.swap(node.node->get()->right);
	return son;
}

unique_ptr<Node> Improver::mutate(const Node& parent) {
	uniform_real_distribution<> mutateDist(0.0, 1.0);
	double d = mutateDist(rand);
	if (d < mConstProb)
		return mutateConst(parent);
	else if (d < mVarProb)
		return mutateVar(parent);
	else if(d<mFuncProb)
		return mutateFunction(parent);
	else if (d <mSubTreeProb)
		return mutateSubTree(parent);
	else if (d < mDelProb)
		return mutateDelete(parent);
	else if (d < mInsProb)
		return mutateInsert(parent);
	return mutateSwap(parent);
}

unique_ptr<Node> Improver::uniteNodes(const Node& parent1, const Node& parent2) {
	unique_ptr<Node> son1 = parent1.clone();
	unique_ptr<Node> son2 = parent2.clone();
	vector<UsingNode> nodes1;
	vector<UsingNode> nodes2;
	usingNodes(nodes1, son1, 1);
	usingNodes(nodes2, son2, 1);
	uniform_int_distribution<> nodeDist1(0, nodes1.size() - 1);
	uniform_int_distribution<> nodeDist2(0, nodes2.size() - 1);
	size_t id1, id2;
	do {
		id1 = nodeDist1(rand);
		id2 = nodeDist2(rand);
	} while (nodes1[id1].depth + nodes2[id2].node->get()->depth() - 1 > maxDepth);
	*nodes1[id1].node = move(*nodes2[id2].node);
	return son1;
}

void Improver::updateProbability(double newBestMeasure) {
	if (newBestMeasure < bestMeasure) {
		bestMeasure = newBestMeasure;
		countGWI = 0;
	}
	else
		countGWI++;
	
	if (initBestMeasure == INFINITY)
		initBestMeasure = newBestMeasure;
	distAdaptation = (log(bestMeasure) - log(target)) / (log(initBestMeasure) - log(target));
	stagAdaptation = min(1.0, (double)countGWI / limitStagnation);
	exploration = 0.8 * distAdaptation + 0.2 * stagAdaptation;

	uniteProbability = 0.5 - 0.2 * stagAdaptation - 0.2 * distAdaptation;
	mutateProbability = uniteProbability + 0.4 - 0.1 * stagAdaptation - 0.1 * distAdaptation;
	mainSelProb = 0.85 - 0.15 * stagAdaptation;

	double localAdaptation = 1.0 - exploration;

	double constProbability = 0.15 + 0.25 * localAdaptation;
	double varProbability = 0.05 + 0.10 * localAdaptation;

	double structuralProbability =
		1.0 - constProbability - varProbability;

	double structuralScale = structuralProbability / 0.55;

	mConstProb = constProbability;
	mVarProb = mConstProb + varProbability;
	mFuncProb = mVarProb + 0.10 * structuralScale;
	mSubTreeProb = mFuncProb + 0.20 * structuralScale;
	mDelProb = mSubTreeProb + 0.10 * structuralScale;
	mInsProb = mDelProb + 0.10 * structuralScale;
	mSwapProb = 1.0;
}

unique_ptr<Node> Improver::improve(const Node& parent1, const Node& parent2) {
	uniform_real_distribution improveDist(0.0, 1.0);
	double d = improveDist(rand);
	if (d < uniteProbability)
		return uniteNodes(parent1, parent2);
	else if (d < mutateProbability)
		return mutate(parent1);
	return generator.generate();
};