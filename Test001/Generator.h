#pragma once
#include "Node.h"
#include "GeneratorConfig.h"
#include <random>

class Generator {
private:
	mt19937 rand;
	size_t maxDepth;
	size_t countVariables;
	GeneratorConfig config;

	unique_ptr<Node> generateTerminal();

	unique_ptr<Node> generateUnarOper(size_t depth);

	unique_ptr<Node> generateBinOper(size_t depth);

public:
	Generator(
		size_t maxDepth,
		size_t countVariables,
		IslandStrategy strategy
	);

	unique_ptr<Node> generate(size_t depth = 1);

	size_t getMaxDepth() const;
};