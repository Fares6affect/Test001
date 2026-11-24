#pragma once
#include "Generator.h"

class Repairer {
private:
	Generator& generator;
	size_t maxDepth;
	mt19937 rand;

	void repairSubTree(
		unique_ptr<Node>& node,
		size_t depth
	);

public:
	Repairer(
		Generator& generator
	);
	void repair(unique_ptr<Node>& node);
};