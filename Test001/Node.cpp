#include "Node.h"
#include <algorithm>
#include <stdexcept>
#include <utility>


Node::Node(Element element): el(element){}

unique_ptr<Node> Node::makeConst(
	double d
) {
	unique_ptr<Node> node = make_unique<Node>(Element::Const);
	node->value = d;
	return node; 
}

unique_ptr<Node> Node::makeVar(
	size_t index
) {
	unique_ptr<Node>  node = make_unique<Node>(Element::Var);
	node->variableIndex = index;
	return node;
}

unique_ptr<Node> Node::makeUnarOper(
	Element oper,
	unique_ptr<Node> left
) {
	if (arity(oper) != 1)
		throw invalid_argument("Неправильное состояние");
	if (!left)
		throw invalid_argument("Операнды не должны быть nullptr");
	unique_ptr<Node>  node = make_unique<Node>(oper);
	node->left = move(left);
	return node;
}

unique_ptr<Node> Node::makeBinOper(
	Element oper,
	unique_ptr<Node> left,
	unique_ptr<Node> right
) {
	if (arity(oper) != 2)
		throw invalid_argument("Неправильное состояние");
	if (!left || !right)
		throw invalid_argument("Операнды не должны быть nullptr");
	unique_ptr<Node>  node = make_unique<Node>(oper);
	node->left = move(left);
	node->right = move(right);
	return node;
}

size_t Node::arity(Element el) {
	switch (el) {
	case Element::Const:
	case Element::Var:
		return 0;

	case Element::Exp:
	case Element::Log:
	case Element::Neg:
	case Element::Conv:
	case Element::Sqrt:
	case Element::Abs:
		return 1;

	case Element::Add:
	case Element::Mul:
		return 2;

	default: throw logic_error("Неизвестное состояние");
	}
}

unique_ptr<Node> Node::clone() const {
	unique_ptr<Node> copy = make_unique<Node>(el);
	copy->value = value;
	copy->variableIndex = variableIndex;
	if (left)
		copy->left = left->clone();
	if (right)
		copy->right = right->clone();
	return copy;
}

size_t Node::size() const {
	size_t res = 1;
	if (left)
		res += left->size();
	if (right)
		res += right->size();
	return res;
}

size_t Node::depth() const {
	return 1 + max(
		left ? left->depth() : 0,
		right ? right->depth() : 0
	);
}