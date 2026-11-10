#include "Simplifier.h"
void Simplifier::simplify(unique_ptr<Node>& node) const {
	switch (node.get()->el) {
	case Element::Const:
	case Element::Var:
		return;
	case Element::Exp: {
		try {
			simplify(node.get()->left);
			if (node.get()->left.get()->el == Element::Const)
				node = Node::makeConst(
					calculator.calculate(
						*node,
						variables
					)
				);
			else if (node.get()->left.get()->el == Element::Log)
				node = move(node.get()->left.get()->left);
			return;
		}
		catch (domain_error&) {
			return;
		}
	}
	case Element::Log: {
		try {
			simplify(node.get()->left);
			if (node.get()->left.get()->el == Element::Const)
				node = Node::makeConst(
					calculator.calculate(
						*node,
						variables
					)
				);
			else if (node.get()->left.get()->el == Element::Exp)
				node = move(node.get()->left.get()->left);
			return;
		}
		catch (domain_error&) {
			return;
		}
	}
	case Element::Neg: {
		try {
			simplify(node.get()->left);
			if (node.get()->left.get()->el == Element::Const)
				node = Node::makeConst(
					calculator.calculate(
						*node,
						variables
					)
				);
			else if (node.get()->left.get()->el == Element::Neg)
				node = move(node.get()->left.get()->left);
			return;
		}
		catch (domain_error&) {
			return;
		}
	}
	case Element::Conv: {
		try {
			simplify(node.get()->left);
			if (node.get()->left.get()->el == Element::Const)
				node = Node::makeConst(
					calculator.calculate(
						*node,
						variables
					)
				);
			else if (node.get()->left.get()->el == Element::Conv)
				node = move(node.get()->left.get()->left);
			return;
		}
		catch (domain_error&) {
			return;
		}
	}
	case Element::Sqrt: {
		try {
			simplify(node.get()->left);
			if (node.get()->left.get()->el != Element::Const)
				return;
			node = Node::makeConst(
				calculator.calculate(
					*node,
					variables
				)
			);
		}
		catch (domain_error&) {
			return;
		}
		return;
	}
	case Element::Add:
	{
		simplify(node->left);
		simplify(node->right);
		if (node->left->el == Element::Const) {
			if (node->left->value == 0.0)
				node = move(node->right);
			else if (node->right->el == Element::Const)
				node = Node::makeConst(
					node->left->value + node->right->value
				);
		}
		else if (node->right->el == Element::Const && node->right->value == 0.0)
			node = move(node->left);
		return;
	}
	case Element::Mul: {
		simplify(node->left);
		simplify(node->right);
		if (node->left->el == Element::Const) {
			if (node->left->value == 0.0)
				node = Node::makeConst(0.0);
			else if (node->left->value == 1.0)
				node = move(node->right);
			else if (node->right->el == Element::Const)
				node = Node::makeConst(
					node->left->value * node->right->value
				);
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