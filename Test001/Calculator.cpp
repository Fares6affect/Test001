#include "Calculator.h"

double Calculator::calculate(
	const Node& node,
	const vector<int>& variables
) const {
	switch (node.el) {
	case Element::Exp:{
		double res = calculate(*node.left, variables);
		if (!isfinite(res))
			throw domain_error("Непредставимое число");
		return exp(res);
	}
	case Element::Log: {
		double res = calculate(*node.left, variables);
		if (!isfinite(res))
			throw domain_error("Непредставимое число");
		if (res <= 0.0)
			throw domain_error(
				"Аргумент логаримфа отрицателен или ноль"
			);
		return log(res);
	}
	case Element::Neg: {
		double res = calculate(*node.left, variables);
		if (!isfinite(res))
			throw domain_error("Непредставимое число");
		return -res;
	}
	case Element::Conv: {
		double res = calculate(*node.left, variables);
		if (!isfinite(res))
			throw domain_error("Непредставимое число");
		if (res == 0.0)
			throw domain_error(
				"Деление на ноль"
			);
		return 1 / (res);
	}
	case Element::Sqrt:
	{
		double res = calculate(*node.left, variables);
		if (!isfinite(res))
			throw domain_error("Непредставимое число");
		if (res < 0.0)
			throw domain_error(
				"Аргумент квадратного корня меньше нуля"
			);
		return sqrt(res);
	}
	case Element::Abs:
	{
		double res = calculate(*node.left, variables);
		if (!isfinite(res))
			throw domain_error("Непредставимое число");
		return abs(res);
	}
	case Element::Add: {
		double res1 = calculate(*node.left, variables);
		if (!isfinite(res1))
			throw domain_error("Непредставимое число");
		double res2 = calculate(*node.right, variables);
		if (!isfinite(res2))
			throw domain_error("Непредставимое число");
		return res1 + res2;
	}
	case Element::Mul: {
		double res1 = calculate(*node.left, variables);
		if (!isfinite(res1))
			throw domain_error("Непредставимое число");
		double res2 = calculate(*node.right, variables);
		if (!isfinite(res2))
			throw domain_error("Непредставимое число");
		return res1 * res2;
	}
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
	case Element::Abs:
		return "|" + getFormula(*node.left) + "|";
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