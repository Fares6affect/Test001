#pragma once
#include "Node.h"
#include <random>

class Generator {
private:
	mt19937 rand;
	const double rad = 100;//Позже проанализировать оптимальные константы
	size_t maxDepth;
	size_t countVariables;

public:
	Generator(
		size_t maxDepth,
		size_t countVariables
	);

	unique_ptr<Node> generateTerminal();

	unique_ptr<Node> generateUnarOper(size_t depth);

	unique_ptr<Node> generateBinOper(size_t depth);

	unique_ptr<Node> generate(size_t depth = 1);
};