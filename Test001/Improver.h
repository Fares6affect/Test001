#pragma once
#include "UsingNode.h"
#include "Generator.h"
#include "IslandStrategy.h"
#include "ImproverConfig.h"
#include "Individual.h"

class Improver {
private:
	mt19937 rand;
	size_t maxDepth;
	size_t sizeSelection;
	size_t countVariables;
	Generator& generator;
	ImproverConfig config;
	double target;

	const array<Element, 6> unarOpers = {
	Element::Exp,
	Element::Log,
	Element::Neg,
	Element::Conv,
	Element::Sqrt,
	Element::Abs
	};

	const array<Element, 2> binOpers = {
		Element::Add,
		Element::Mul
	};

	double distAdaptation = 1.0;
	double stagAdaptation = 0.0;
	double exploration = 1.0;

	size_t countGWI = 0;
	size_t limitStagnation = 10;
	double bestMeasure = +INFINITY;
	double initBestMeasure = +INFINITY;

	double uniteProbability = 0.0;
	double mutateProbability = 0.0;

	double mainSelProb = 0.0;

	double mConstProb = 0.0;
	double mVarProb = 0.0;
	double mFuncProb = 0.0;
	double mSubTreeProb = 0.0;
	double mDelProb = 0.0;
	double mInsProb = 0.0;
	double mSwapProb = 0.0;

public:

	Improver(
		size_t maxDepth,
		size_t sizeSelection,
		size_t countVariables,
		Generator& generator,
		IslandStrategy strategy,
		double target
	);

	size_t selection(
		const vector<Individual>& individuals
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