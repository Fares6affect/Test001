#include "Simplifier.h"
void Simplifier::simplify(unique_ptr<Node>& node) const {
	switch (node.get()->el) {

	case Element::Const:
	case Element::Var:
		return;

	case Element::Exp: {
		simplify(node.get()->left);
		if (node.get()->left.get()->el == Element::Const) {
			CalcResult cr = calculator.tryCalculate(*node, {});
			if (cr.isValid())
				node = Node::makeConst(cr.value);
		}
		else if (node.get()->left.get()->el == Element::Log)
			node = move(node.get()->left.get()->left);
		return;
	}

	case Element::Log: {
		simplify(node.get()->left);
		if (node.get()->left.get()->el == Element::Const) {
			CalcResult cr = calculator.tryCalculate(*node, {});
			if (cr.isValid())
				node = Node::makeConst(cr.value);
		}
		else if (node.get()->left.get()->el == Element::Exp)
			node = move(node.get()->left.get()->left);
		return;
	}

	case Element::Neg: {
		simplify(node.get()->left);
		if (node.get()->left.get()->el == Element::Const) {
			CalcResult cr = calculator.tryCalculate(*node, {});
			if (cr.isValid())
				node = Node::makeConst(cr.value);
		}
		else if (node.get()->left.get()->el == Element::Neg)
			node = move(node.get()->left.get()->left);
		return;
	}

	case Element::Conv: {
		simplify(node.get()->left);
		if (node.get()->left.get()->el == Element::Const) {
			CalcResult cr = calculator.tryCalculate(*node, {});
			if (cr.isValid())
				node = Node::makeConst(cr.value);
		}
		else if (node.get()->left.get()->el == Element::Conv)
			node = move(node.get()->left.get()->left);
		return;
	}

	case Element::Sqrt: {
		simplify(node.get()->left);
		if (node.get()->left.get()->el == Element::Const) {
			CalcResult cr = calculator.tryCalculate(*node, {});
			if (cr.isValid())
				node = Node::makeConst(cr.value);
		}
		return;
	}

	case Element::Abs: {
		simplify(node.get()->left);
		if (node.get()->left.get()->el == Element::Const) {
			CalcResult cr = calculator.tryCalculate(*node, {});
			if (cr.isValid())
				node = Node::makeConst(cr.value);
		}
		else if (node.get()->left.get()->el == Element::Neg)
			node.get()->left = move(node.get()->left.get()->left);
		return;
	}

	case Element::Add:
	{
		simplify(node->left);
		simplify(node->right);
		if (node.get()->left.get()->el == Element::Const) {
			if (node->left->value == 0.0)
				node = move(node->right);
			else if (node->right->el == Element::Const) {
				CalcResult cr = calculator.tryCalculate(*node, {});
				if (cr.isValid())
					node = Node::makeConst(cr.value);
			}
		}
		else if (node->right->el == Element::Const && node->right->value == 0.0)
			node = move(node->left);
		return;
	}

	case Element::Mul: {
		simplify(node->left);
		simplify(node->right);
		if (node.get()->left.get()->el == Element::Const) {
			if (node->left->value == 0.0)
				node = Node::makeConst(0.0);
			else if (node->left->value == 1.0)
				node = move(node->right);
			else if (node->right->el == Element::Const) {
				CalcResult cr = calculator.tryCalculate(*node, {});
				if (cr.isValid())
					node = Node::makeConst(cr.value);
			}
		}
		else if (node->right->el == Element::Const)
			if (node->right->value == 0.0)
				node = Node::makeConst(0.0);
			else if (node->right->value == 1.0)
				node = move(node->left);
		return;
	}

	}
}