#pragma once
#include "Node.h"

struct Individual
{
	unique_ptr<Node> node;
	double measure;

	Individual(
		unique_ptr<Node> node,
		double measure
	):
		node(move(node)),
		measure(measure){}
};