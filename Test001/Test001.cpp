#include <iostream>
#include <vector>
#include <string>
using namespace std;

enum class Element {
	Exp,
	Log,
	Neg,
	Conv,
	Sqrt,

	Add,
	Mul,

	Const,
	Var
};

struct Node
{
	Element el;
	double value = 0.0;
	size_t variableIndex = 0;
	unique_ptr<Node> left;
	unique_ptr<Node> right;

	static unique_ptr<Node> makeConst(
		double d
	) {
		unique_ptr<Node>  node = make_unique<Node>(Element::Const);
		node->value = d;
		return node;
	}

	static unique_ptr<Node> makeVar(
		size_t index
	) {
		unique_ptr<Node>  node = make_unique<Node>(Element::Var);
		node->variableIndex = index;
		return node;
	}

	static unique_ptr<Node> makeBinOper(
		Element oper,
		unique_ptr<Node> left,
		unique_ptr<Node> right
	) {
		unique_ptr<Node>  node = make_unique<Node>(oper);
		node->left = move(left);
		node->right = move(right);
		return node;
	}

	static unique_ptr<Node> makeUnarOper(
		Element oper,
		unique_ptr<Node> left
	) {
		unique_ptr<Node>  node = make_unique<Node>(oper);
		node->left = move(left);
		return node;
	}
};

class Calculator {
public:
	double calculate(
		const Node& node,
		const vector<int>& variables
	) {
		switch (node.el) {
			case Element::Exp:
				return exp(calculate(*node.left, variables));
			case Element::Log: {
				double res = calculate(*node.left, variables);
				if (res <= 0.0)
					throw domain_error(
						"Log is negative"
				);
				return log(res);
			}
			case Element::Neg:
				return -calculate(*node.left, variables);
			case Element::Conv: {
				double res = calculate(*node.left, variables);
				if (res == 0.0)
					throw domain_error(
						"Devide on 0"
					);
				return 1/(res);
			}
			case Element::Sqrt:
			{
				double res = calculate(*node.left, variables);
				if (res < 0.0)
					throw domain_error(
						"Sqrt is negative"
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
						"Variable index is out of range"
					);
				return variables[node.variableIndex];
			}
			default: throw logic_error("Unknown element");
		}
	}
	string getFormula(
		const Node& node
	) {
		switch (node.el) {
		case Element::Exp:
			return "e^(" + getFormula(*node.left)+")";
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
			return "x"+to_string(node.variableIndex);
		default: throw logic_error("Unknown element");
		}
	}
	string getOptimizeFormula(
		string str
	) {
		
	}
};

int main()
{
	unique_ptr<Node> x1 = Node::makeVar(0);
	unique_ptr<Node> y = Node::makeVar(1);
	unique_ptr<Node> negativeY = Node::makeUnarOper(
		Element::Neg,
		move(y)
	);
	unique_ptr<Node> add = Node::makeBinOper(
		Element::Add,
		move(x1),
		move(negativeY)
	);
	unique_ptr<Node> x2 = Node::makeVar(1);
	unique_ptr<Node> negativeX2 = Node::makeUnarOper(
		Element::Neg,
		move(x2)
	);
	unique_ptr<Node> mult = Node::makeBinOper(
		Element::Mul,
		move(add),
		move(negativeX2)
	);
	Calculator calculator;
	int n, m;
	cin >> n >> m;
	vector<int> variables{ n, m };
	cout << calculator.getFormula(*mult) << endl;
	cout << calculator.calculate(*mult, variables);

}