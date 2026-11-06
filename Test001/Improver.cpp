#include "Improver.h"

Improver::Improver(
	size_t maxDepth,
	size_t sizeSelection,
	size_t countVariables,
	Generator& generator
) :rand(random_device{}()),
maxDepth(maxDepth),
sizeSelection(sizeSelection),
generator(generator)
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
	uniform_int_distribution<> idParent(0, measures.size() - 1);
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
	size_t  id = nodeDist(rand);
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
		if (node.depth + node.node->get()->depth() < maxDepth)
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
	uniform_int_distribution<> mutateDist(0, 19);
	switch (mutateDist(rand)) {
	case 0:
	case 1:
	case 2:
		return mutateFunction(parent);//15% мутации функции
	case 3:
	case 4:
	case 5:
	case 6:
		return mutateSubTree(parent);//20% мутации поддерева
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		return mutateDelete(parent);//25% мутации удаления узла
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
		return mutateInsert(parent);//25% мутации добавления узла
	case 17:
	case 18:
	case 19:
		return mutateSwap(parent);//15% мутации замени потомков
	}
}