#pragma once
#include "Node.h"

struct Individual
{
	unique_ptr<Node> tree;
	double measure;
};