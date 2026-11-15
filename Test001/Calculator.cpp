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
		return fabs(res);
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
		return "(-(" + getFormula(*node.left) + "))";
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

vector<SimpleElement> Calculator::getElements(
	const Node& node
) const {
	switch (node.el) {
	case Element::Exp: {
		vector<SimpleElement> resSon = getElements(*node.left);
		string strResSon = (resSon[0].sign ? "-" : "") + resSon[0].element;
		for (size_t i =1;i<resSon.size();i++)
			strResSon += (resSon[i].sign?"-":"+") + resSon[i].element;
		SimpleElement se("e^(" + strResSon + ")", false);
		vector<SimpleElement> res = { se };
		return res;
	}
	case Element::Log: {
		vector<SimpleElement> resSon = getElements(*node.left);
		string strResSon = (resSon[0].sign ? "-" : "") + resSon[0].element;
		for (size_t i = 1;i < resSon.size();i++)
			strResSon += (resSon[i].sign ? "-" : "+") + resSon[i].element;
		SimpleElement se("log(" + strResSon + ")", false);
		vector<SimpleElement> res = { se };
		return res;
	}
	case Element::Neg: {
		vector<SimpleElement> resSon = getElements(*node.left);
		for (size_t i = 0;i < resSon.size();i++)
			resSon[i].sign = !resSon[i].sign;
		return resSon;
	}
	case Element::Conv: {
		vector<SimpleElement> resSon = getElements(*node.left);
		string strResSon = (resSon[0].sign ? "-" : "") + resSon[0].element;
		for (size_t i = 1;i < resSon.size();i++)
			strResSon += (resSon[i].sign ? "-" : "+") + resSon[i].element;
		SimpleElement se("1/(" + strResSon + ")", false);
		vector<SimpleElement> res = { se };
		return res;
	}
	case Element::Sqrt: {
		vector<SimpleElement> resSon = getElements(*node.left);
		string strResSon = (resSon[0].sign ? "-" : "") + resSon[0].element;
		for (size_t i = 1;i < resSon.size();i++)
			strResSon += (resSon[i].sign ? "-" : "+") + resSon[i].element;
		SimpleElement se("sqrt(" + strResSon + ")", false);
		vector<SimpleElement> res = { se };
		return res;
	}
	case Element::Abs: {
		vector<SimpleElement> resSon = getElements(*node.left);
		string strResSon = (resSon[0].sign ? "-" : "") + resSon[0].element;
		for (size_t i = 1;i < resSon.size();i++)
			strResSon += (resSon[i].sign ? "-" : "+") + resSon[i].element;
		SimpleElement se("|" + strResSon + "|", false);
		vector<SimpleElement> res = { se };
		return res;
	}
	case Element::Add: {
		vector<SimpleElement> resSon1 = getElements(*node.left);
		vector<SimpleElement> resSon2 = getElements(*node.right);
		size_t resSon1Size = resSon1.size();
		size_t resSon2Size = resSon2.size();
		vector<SimpleElement> res;
		res.reserve(resSon1Size + resSon2Size);
		for (size_t i = 0;i < resSon1Size;i++)
			res.push_back(resSon1[i]);
		for (size_t i = 0;i < resSon2Size;i++)
			res.push_back(resSon2[i]);
		return res;
	}
	case Element::Mul: {
		vector<SimpleElement> resSon1 = getElements(*node.left);
		vector<SimpleElement> resSon2 = getElements(*node.right);
		size_t resSon1Size = resSon1.size();
		size_t resSon2Size = resSon2.size();
		vector<SimpleElement> res;
		res.reserve(resSon1Size * resSon2Size);
		for (size_t i = 0;i < resSon1Size;i++)
			for (size_t j = 0;j < resSon2Size;j++) {
				SimpleElement se(resSon1[i].element+"*"+resSon2[j].element, resSon1[i].sign ^ resSon2[j].sign);
				res.push_back(se);
			}
		return res;
	}
	case Element::Const: {
		SimpleElement se(to_string(fabs(node.value)), node.value<0.0);
		vector<SimpleElement> res = { se };
		return res;
	}
	case Element::Var: {
		SimpleElement se("x" + to_string(node.variableIndex), false);
		vector<SimpleElement> res = { se };
		return res;
	}
	}
}

string Calculator::getOptimizeFormula(
	const Node& node
) const {
	vector<SimpleElement> elements = getElements(node);
	string res = (elements[0].sign ? "-" : "") + elements[0].element;
	for (size_t i = 1;i<elements.size();i++)
		res += (elements[i].sign ? "-" : "+") + elements[i].element;
	return res;
}