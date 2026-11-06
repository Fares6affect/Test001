#pragma once
#include "UsingNode.h"
#include "Generator.h"

class Improver {
private:
	mt19937 rand;
	size_t maxDepth;
	size_t count;
	Generator generator;

public:

	Improver(
		size_t maxDepth,
		size_t count,
		size_t countVariables
	);

	size_t selection(
		const vector<double>& measures
	);

	void usingNodes(
		vector<UsingNode>& nodes,
		unique_ptr<Node>& node,
		size_t depth
	);

	unique_ptr<Node> mutateFunction(const Node& parent);

	unique_ptr<Node> mutateSubTree(const Node& parent);

	unique_ptr<Node> mutateDelete(const Node& parent);

	unique_ptr<Node> mutateInsert(const Node& parent);

	unique_ptr<Node> mutateSwap(const Node& parent);

	unique_ptr<Node> mutate(const Node& parent);

};