#include "Calculator.h"
#include <cmath>

double Calculator::calculate(
	const Node& node,
	const vector<int>& variables
) const {
	switch (node.el) {
	case Element::Exp:
		return exp(calculate(*node.left, variables));
	case Element::Log: {
		double res = calculate(*node.left, variables);
		if (res <= 0.0)
			throw domain_error(
				"Аргумент логаримфа отрицателен или ноль"
			);
		return log(res);
	}
	case Element::Neg:
		return -calculate(*node.left, variables);
	case Element::Conv: {
		double res = calculate(*node.left, variables);
		if (res == 0.0)
			throw domain_error(
				"Деление на ноль"
			);
		return 1 / (res);
	}
	case Element::Sqrt:
	{
		double res = calculate(*node.left, variables);
		if (res < 0.0)
			throw domain_error(
				"Аргумент квадратного корня меньше нуля"
			);
		return sqrt(res);
	}
	case Element::Add:
		return calculate(*node.left, variables) + calculate(*node.right, variables);
	case Element::Mul:
		return calculate(*node.left, variables) * calculate(*node.right, variables);
	case Element::Const:
		return node.value;
	case Element::Var: {
		if (node.variableIndex >= variables.size())
			throw out_of_range(
				"Выход за границы вектора"
			);
		return variables[node.variableIndex];
	}
	default: throw logic_error("Неизвестное состояние");
	}
}
string Calculator::getFormula(
	const Node& node
) const {
	switch (node.el) {
	case Element::Exp:
		return "e^(" + getFormula(*node.left) + ")";
	case Element::Log:
		return "log^(" + getFormula(*node.left) + ")";
	case Element::Neg:
		return "-(" + getFormula(*node.left) + ")";
	case Element::Conv:
		return "1/(" + getFormula(*node.left) + ")";
	case Element::Sqrt:
		return "sqrt(" + getFormula(*node.left) + ")";
	case Element::Add:
		return "(" + getFormula(*node.left) + "+" + getFormula(*node.right) + ")";
	case Element::Mul:
		return "(" + getFormula(*node.left) + ")*(" + getFormula(*node.right) + ")";
	case Element::Const:
		return to_string(node.value);
	case Element::Var:
		return "x" + to_string(node.variableIndex);
	default: throw logic_error("Неизвестное состояние");
	}
}
string Calculator::getOptimizeFormula(
	const Node& node
) const {
	switch (node.el) {
	case Element::Exp:
		return "e^(" + getOptimizeFormula(*node.left) + ")";
	case Element::Log:
		return "log(" + getOptimizeFormula(*node.left) + ")";
	case Element::Neg:
	{
		const Node& child = *node.left;

		if (child.el == Element::Const) {
			return to_string(-child.value);
		}

		if (child.el == Element::Neg) {
			return getOptimizeFormula(*child.left);
		}

		return "-(" + getOptimizeFormula(child) + ")";
	}
	case Element::Conv:
		return "1/(" + getOptimizeFormula(*node.left) + ")";
	case Element::Sqrt:
		return "sqrt(" + getOptimizeFormula(*node.left) + ")";
	case Element::Add:
	{
		string res = getOptimizeFormula(*node.left) + "+" + getOptimizeFormula(*node.right);
		for (int i = 0; i < res.size() - 1; i++)
			if (
				res[i] == '+' && res[i + 1] == '-' || res[i] == '-' && res[i + 1] == '+'
				)
				res.replace(i, 2, "-");
		return res;
	}
	case Element::Mul: {
		return "(" + getOptimizeFormula(*node.left) + ")*(" + getOptimizeFormula(*node.right) + ")";
	}
	case Element::Const:
		return to_string(node.value);
	case Element::Var:
		return "x" + to_string(node.variableIndex);
	default: throw logic_error("Неизвестное состояние");
	}
}