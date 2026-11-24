#include "Repairer.h"

Repairer::Repairer(
	Generator& generator
):generator(generator),
maxDepth(generator.getMaxDepth()),
rand(random_device{}()) {}

void Repairer::repairSubTree(
	unique_ptr<Node>& node,
	size_t depth
) {
	if (!node) return;
	if (node.get()->arity(node.get()->el)!=0) {
		if (node.get()->variableIndex > 0) {
			bernoulli_distribution genDist(0.9);
			bernoulli_distribution changeDist(0.9);
			switch (node.get()->variableIndex) {
			case 1:
			case 2: {
				if (genDist(rand)) {
					if (depth + node.get()->depth() + 1 <= maxDepth && changeDist(rand)) {
						unique_ptr<Node> absNode = Node::makeUnarOper(
							Element::Abs,
							move(node.get()->left)
						);
						unique_ptr<Node> addNode = Node::makeBinOper(
							Element::Add,
							move(absNode),
							Node::makeConst(1.0)
						);
						node.get()->left = move(addNode);
						node.get()->variableIndex = 0;
						return;
					}
					else node = move(node.get()->left);
				}
				else node = generator.generate(depth);
				return;
			}
			case 3: {
				if (genDist(rand)) {
					if (depth + node.get()->depth() <= maxDepth && changeDist(rand)) {
						unique_ptr<Node> absNode = Node::makeUnarOper(
							Element::Abs,
							move(node.get()->left)
						);
						node.get()->left = move(absNode);
						node.get()->variableIndex = 0;
						return;
					}
					node = move(node.get()->left);
				}
				else node = generator.generate(depth);
				return;
			}
			case 4: {
				switch (node.get()->el) {
				case Element::Exp: {
					if (genDist(rand))
						node = move(node.get()->left);
					else
						node = generator.generate(depth);
					return;
				}
				case Element::Conv: {
					if (depth + node.get()->depth() + 1 <= maxDepth && genDist(rand)) {
						unique_ptr<Node> absNode = Node::makeUnarOper(
							Element::Abs,
							move(node.get()->left)
						);
						unique_ptr<Node> addNode = Node::makeBinOper(
							Element::Add,
							move(absNode),
							Node::makeConst(1.0)
						);
						node.get()->left = move(addNode);
						node.get()->variableIndex = 0;
						return;
					}
					node = move(node.get()->left);
					return;
				}
				case Element::Add:
				case Element::Mul: {
					bernoulli_distribution sideDist(0.5);
					if (genDist(rand))
						if (sideDist(rand))
							node = move(node.get()->left);
						else
							node = move(node.get()->right);
					else
						node = generator.generate(depth);
					return;
				}
				}
			}
			}
		}
		repairSubTree(node.get()->left, depth + 1);
		repairSubTree(node.get()->right, depth + 1);
	}
}

void Repairer::repair(
	unique_ptr<Node>& node
) {
	repairSubTree(node, 1);
}