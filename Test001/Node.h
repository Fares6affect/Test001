#pragma once
#include <cstddef>
#include <memory>
#include "Element.h"
using namespace std;

class Node
{
public:
	Element el;
	double value = 0.0;
	size_t variableIndex = 0;
	unique_ptr<Node> left;
	unique_ptr<Node> right;

	Node(Element element);

	static unique_ptr<Node> makeConst(
		double d
	);

	static unique_ptr<Node> makeVar(
		size_t index
	);

	static unique_ptr<Node> makeUnarOper(
		Element oper,
		unique_ptr<Node> left
	);

	static unique_ptr<Node> makeBinOper(
		Element oper,
		unique_ptr<Node> left,
		unique_ptr<Node> right
	);

	static size_t arity(Element el);

	bool isTerminal() const;

	unique_ptr<Node> clone() const;

	size_t size() const;

	size_t depth() const;
};