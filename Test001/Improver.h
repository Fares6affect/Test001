#pragma once
#include "UsingNode.h"
#include "Generator.h"

class Improver {
private:
	mt19937 rand;
	size_t maxDepth;
	size_t sizeSelection;
	size_t countVariables;
	Generator& generator;
	double target;

	double distAdaptation = 1.0;
	double stagAdaptation = 0.0;
	double exploration = 1.0;

	size_t countGWI = 0;
	size_t limitStagnation = 10;
	double bestMeasure = +INFINITY;
	double initBestMeasure = +INFINITY;

	double mainSelProb = 0.85;

	double uniteProbability = 0.4;
	double mutateProbability = uniteProbability + 0.4;

	double mConstProb = 0.3;
	double mVarProb = mConstProb + 0.15;
	double mFuncProb = mVarProb + 0.1;
	double mSubTreeProb = mFuncProb + 0.2;
	double mDelProb = mSubTreeProb + 0.1;
	double mInsProb = mDelProb + 0.1;
	double mSwapProb = mInsProb + 0.05;

public:

	Improver(
		size_t maxDepth,
		size_t sizeSelection,
		size_t countVariables,
		Generator& generator,
		double target
	);

	size_t selection(
		const vector<double>& measures
	);

	void usingNodes(
		vector<UsingNode>& nodes,
		unique_ptr<Node>& node,
		size_t depth
	);

	unique_ptr<Node> mutateConst(const  Node& parent);

	unique_ptr<Node> mutateVar(const  Node& parent);

	unique_ptr<Node> mutateFunction(const Node& parent);

	unique_ptr<Node> mutateSubTree(const Node& parent);

	unique_ptr<Node> mutateDelete(const Node& parent);

	unique_ptr<Node> mutateInsert(const Node& parent);

	unique_ptr<Node> mutateSwap(const Node& parent);

	unique_ptr<Node> mutate(const Node& parent);

	unique_ptr<Node> uniteNodes(const Node& parent1, const Node& parent2);

	void updateProbability(double BestMeasure);

	unique_ptr<Node> improve(const Node& parent1, const Node& parent2);
};