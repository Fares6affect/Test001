#include "Calculator.h"

CalcResult Calculator::tryCalculate(
	const Node& node,
	const vector<int>& variables
) const {
	switch (node.el) {

	case Element::Const:
		return { node.value, CalcStatus::Valid };

	case Element::Var:
		return { static_cast<double>(variables[node.variableIndex]), CalcStatus::Valid };

	case Element::Exp: {
		CalcResult cr = tryCalculate(*node.left, variables);
		if (!cr.isValid()) return cr;
		double res = exp(cr.value);
		if (!isfinite(res)) return { 0.0, CalcStatus::NonFinite };
		return { res, CalcStatus::Valid };
	}

	case Element::Log: {
		CalcResult cr = tryCalculate(*node.left, variables);
		if (!cr.isValid()) return cr;
		if (cr.value <= 0.0) return { cr.value, CalcStatus::LogError };
		return { log(cr.value), CalcStatus::Valid };
	}

	case Element::Neg: {
		CalcResult cr = tryCalculate(*node.left, variables);
		if (!cr.isValid()) return cr;
		return { -cr.value, CalcStatus::Valid };
	}

	case Element::Conv: {
		CalcResult cr = tryCalculate(*node.left, variables);
		if (!cr.isValid()) return cr;
		if (cr.value == 0.0) return { 0.0, CalcStatus::ConvError };
		return { 1/(cr.value), CalcStatus::Valid };
	}

	case Element::Sqrt: {
		CalcResult cr = tryCalculate(*node.left, variables);
		if (!cr.isValid()) return cr;
		if (cr.value < 0.0) return { 0.0, CalcStatus::SqrtError };
		return { sqrt(cr.value), CalcStatus::Valid };
	}

	case Element::Abs: {
		CalcResult cr = tryCalculate(*node.left, variables);
		if (!cr.isValid()) return cr;
		return { fabs(cr.value), CalcStatus::Valid };
	}

	case Element::Add: {
		CalcResult cr1 = tryCalculate(*node.left, variables);
		if (!cr1.isValid()) return cr1;
		CalcResult cr2 = tryCalculate(*node.right, variables);
		if (!cr2.isValid()) return cr2;
		double res = cr1.value + cr2.value;
		if(!isfinite(res)) return {0.0,CalcStatus::NonFinite};
		return { res, CalcStatus::Valid };
	}

	case Element::Mul: {
		CalcResult cr1 = tryCalculate(*node.left, variables);
		if (!cr1.isValid()) return cr1;
		CalcResult cr2 = tryCalculate(*node.right, variables);
		if (!cr2.isValid()) return cr2;
		double res = cr1.value * cr2.value;
		if (!isfinite(res)) return { 0.0,CalcStatus::NonFinite };
		return { res, CalcStatus::Valid };
	}

	}
}

string Calculator::getFormula(
	const Node& node
) const {
	switch (node.el) {
	case Element::Exp:
		return "e^(" + getFormula(*node.left) + ")";
	case Element::Log:
		return "log(" + getFormula(*node.left) + ")";
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

vector<SimpleElement> Calculator::simplifyElements(
	const vector<SimpleElement>& elements
) const {
	map<vector<string>,double> groups;
	for (const SimpleElement& element : elements) {
		vector<string> mns = element.mns;
		sort(mns.begin(), mns.end());
		groups[mns] += element.k;
	}
	vector<SimpleElement> res;
	for (const auto& [mns, k] : groups) {
		if (k == 0.0) continue;
		res.emplace_back(k, mns);
	}
	return res;
}

vector<SimpleElement> Calculator::getElements(
	const Node& node
) const {
	switch (node.el) {
	case Element::Const: {
		return {
			SimpleElement(node.value, {})
		};
	}
	case Element::Var: {
		return {
			SimpleElement(1.0, {"x" + to_string(node.variableIndex)})
		};
	}

	case Element::Exp:
		return { SimpleElement(1.0,{"e^(" + getOptimizeFormula(*node.left) + ")"}) };
	case Element::Log:
		return { SimpleElement(1.0,{"log(" + getOptimizeFormula(*node.left) + ")"}) };
	case Element::Conv:
		return { SimpleElement(1.0,{"1/(" + getOptimizeFormula(*node.left) + ")"}) };
	case Element::Sqrt:
		return { SimpleElement(1.0,{"sqrt(" + getOptimizeFormula(*node.left) + ")"}) };
	case Element::Abs: {
		return { SimpleElement(1.0,{"|" + getOptimizeFormula(*node.left) + "|"}) };
	}
	case Element::Neg: {
		vector<SimpleElement> res = getElements(*node.left);
		for (SimpleElement& element : res)
			element.k *= -1.0;
		return res;
	}

	case Element::Add: {
		vector<SimpleElement> left = getElements(*node.left);
		vector<SimpleElement> right = getElements(*node.right);
		left.insert(left.end(), right.begin(), right.end());
		return simplifyElements(left);
	}
	case Element::Mul: {
		vector<SimpleElement> left = getElements(*node.left);
		vector<SimpleElement> right = getElements(*node.right);
		vector<SimpleElement> res;
		res.reserve(left.size() * right.size());
		for(const SimpleElement& a:left)
			for (const SimpleElement& b : right) {
				vector<string> mns = a.mns;
				mns.insert(mns.end(), b.mns.begin(), b.mns.end());
				res.emplace_back(a.k * b.k, mns);
			}
		return simplifyElements(res);
	}
	}
}

string Calculator::getOptimizeFormula(
	const Node& node
) const {
	vector<SimpleElement> elements = simplifyElements(getElements(node));
	if (elements.empty())
		return "0";
	ostringstream stream;
	for (size_t i = 0; i < elements.size(); i++) {
		const SimpleElement& element = elements[i];
		double k = element.k;
		if (i > 0)
			stream << (k < 0.0 ? "-" : "+");
		else stream << (k < 0.0 ? "-" : "");
		k = fabs(k);
		if (element.mns.empty()) {
			stream << setprecision(numeric_limits<double>::max_digits10) << k;
			continue;
		}
		if (k!=1.0)
			stream << setprecision(numeric_limits<double>::max_digits10) << k << "*";
		for (size_t j = 0; j < element.mns.size(); j++) {
			if (j > 0)
				stream << "*";
			stream << element.mns[j];
		}
	}
	return stream.str();
}