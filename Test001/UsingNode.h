#pragma once
#include "Node.h"

struct UsingNode {
	unique_ptr<Node>* node;
	size_t depth;
};