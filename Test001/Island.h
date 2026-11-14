#pragma once
#include "Simplifier.h"
#include "Population.h"
#include "Generator.h"
#include "Improver.h"
#include "Meter.h"
#include "ConstOptimizer.h"

class Island {
private:
	size_t sizePopulation;//Размер каждой популяции
	size_t elitismCount;//Количество родителей переносимых в следующее поколение

	unique_ptr<Generator> generator;
	unique_ptr<Improver> improver;
	Simplifier simplifier;
	Meter& meter;
	ConstOptimizer constOptimizer;

	unique_ptr<Population> population;

public:
	Island(
		size_t sizePopulation,
		size_t sizeSelection,
		size_t elitismCount,

		size_t maxDepth,
		size_t countVariables,
		Meter& meter,
		IslandStrategy strategy,

		double target
	);

	void createPopulation();

	unique_ptr<Node> getBestNode() const;

	vector<unique_ptr<Node>> getBestNodes(size_t count) const;
	
	double getBestMeasure() const;

	void migration(unique_ptr<Node> migrant);

	void migrations(vector<unique_ptr<Node>> migrants);

	void nextPopulation();
};