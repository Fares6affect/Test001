#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <fstream>
using namespace std;

enum class Element {
	Const,
	Var,

	Exp,
	Log,
	Neg,
	Conv,
	Sqrt,

	Add,
	Mul
};

class Node
{
	friend class Simplifier;
public:
	Element el;
	double value = 0.0;
	size_t variableIndex = 0;
	unique_ptr<Node> left;
	unique_ptr<Node> right;
	Node(Element element) {
		el = element;
	}

	const Element getElement() const{ return el; }

	const double getValue() const{ return value; }
	void setValue(double value) { this->value = value; }

	const size_t getVariableIndex() const{ return variableIndex; }

	const Node* getLeft() const { return left.get(); }
	void setLeft(unique_ptr<Node> node) { left = move(node); }

	const Node* getRight() const { return right.get(); }
	void setRight(unique_ptr<Node> node) { right = move(node); }


	static size_t arity(Element el) {
		switch (el) {
		case Element::Const:
		case Element::Var:
			return 0;

		case Element::Exp:
		case Element::Log:
		case Element::Neg:
		case Element::Conv:
		case Element::Sqrt:
			return 1;

		case Element::Add:
		case Element::Mul:
			return 2;

		default: throw logic_error("Неизвестное состояние");
		}
	}

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
		if (!left || !right) {
			throw std::invalid_argument("Операнды не должны быть nullptr");
		}
		if (arity(oper) == 2) {
			unique_ptr<Node>  node = make_unique<Node>(oper);
			node->left = move(left);
			node->right = move(right);
			return node;
		}
		throw "Неправильное состояние";
	}

	static unique_ptr<Node> makeUnarOper(
		Element oper,
		unique_ptr<Node> left
	) {
		if (arity(oper) == 1) {
			unique_ptr<Node>  node = make_unique<Node>(oper);
			node->left = move(left);
			return node;
		}
		throw "Неправильное состояние";
	}

	bool isTerminal() const {
		return el == Element::Const || el == Element::Var;
	}

	unique_ptr<Node> clone() const{
		unique_ptr<Node> copy = make_unique<Node>(el);
		copy->value = value;
		copy->variableIndex = variableIndex;
		if (left)
			copy->left = left->clone();
		if (right)
			copy->right = right->clone();
		return copy;
	}

	size_t size() const{
		size_t res = 1;
		if (left)
			res += left->size();
		if (right)
			res += right->size();
		return res;
	}

	size_t depth() const{
		return 1 + max(
			left ? left->depth() : 0,
			right ? right->depth() : 0
		);
	}
};

class Calculator {
public:
	double calculate(
		const Node& node,
		const vector<int>& variables
	) const{
		switch (node.getElement()) {
			case Element::Exp:
				return exp(calculate(*node.getLeft(), variables));
			case Element::Log: {
				double res = calculate(*node.getLeft(), variables);
				if (res <= 0.0)
					throw domain_error(
						"Аргумент логаримфа отрицателен или ноль"
				);
				return log(res);
			}
			case Element::Neg:
				return -calculate(*node.getLeft(), variables);
			case Element::Conv: {
				double res = calculate(*node.getLeft(), variables);
				if (res == 0.0)
					throw domain_error(
						"Деление на ноль"
					);
				return 1/(res);
			}
			case Element::Sqrt:
			{
				double res = calculate(*node.getLeft(), variables);
				if (res < 0.0)
					throw domain_error(
						"Аргумент квадратного корня меньше нуля"
					);
				return sqrt(res);
			}
			case Element::Add:
				return calculate(*node.getLeft(), variables) + calculate(*node.getRight(), variables);
			case Element::Mul:
				return calculate(*node.getLeft(), variables) * calculate(*node.getRight(), variables);
			case Element::Const:
				return node.getValue();
			case Element::Var: {
				if (node.getVariableIndex() >= variables.size())
					throw out_of_range(
						"Выход за границы вектора"
					);
				return variables[node.getVariableIndex()];
			}
			default: throw logic_error("Неизвестное состояние");
		}
	}
	string getFormula(
		const Node& node
	) const {
		switch (node.getElement()) {
		case Element::Exp:
			return "e^(" + getFormula(*node.getLeft())+")";
		case Element::Log:
			return "log^(" + getFormula(*node.getLeft()) + ")";
		case Element::Neg:
			return "-(" + getFormula(*node.getLeft()) + ")";
		case Element::Conv:
			return "1/(" + getFormula(*node.getLeft()) + ")";
		case Element::Sqrt:
			return "sqrt(" + getFormula(*node.getLeft()) + ")";
		case Element::Add:
			return "(" + getFormula(*node.getLeft()) + "+" + getFormula(*node.getRight()) + ")";
		case Element::Mul:
			return "(" + getFormula(*node.getLeft()) + ")*(" + getFormula(*node.getRight()) + ")";
		case Element::Const:
			return to_string(node.getValue());
		case Element::Var:
			return "x"+to_string(node.getVariableIndex());
		default: throw logic_error("Неизвестное состояние");
		}
	}
	string getOptimizeFormula(
		const Node &node
	) const{
		switch (node.getElement()) {
		case Element::Exp:
			return "e^(" + getOptimizeFormula(*node.getLeft()) + ")";
		case Element::Log:
			return "log(" + getOptimizeFormula(*node.getLeft()) + ")";
		case Element::Neg:
		{
			const Node& child = *node.getLeft();

			if (child.getElement() == Element::Const) {
				return to_string(-child.getValue());
			}

			if (child.getElement() == Element::Neg) {
				return getOptimizeFormula(*child.getLeft());
			}

			return "-(" + getOptimizeFormula(child) + ")";
		}
		case Element::Conv:
			return "1/(" + getOptimizeFormula(*node.getLeft()) + ")";
		case Element::Sqrt:
			return "sqrt(" + getOptimizeFormula(*node.getLeft()) + ")";
		case Element::Add:
		{
			string res = getOptimizeFormula(*node.getLeft()) + "+" + getOptimizeFormula(*node.getRight());
			for (int i = 0;i < res.size() - 1;i++)
				if (
					res[i] == '+' && res[i + 1] == '-' || res[i] == '-' && res[i + 1] == '+'
					)
					res.replace(i, 2, "-");
			return res;
		}
		case Element::Mul: {
			return "(" + getOptimizeFormula(*node.getLeft()) + ")*(" + getOptimizeFormula(*node.getRight()) + ")";
		}
		case Element::Const:
			return to_string(node.getValue());
		case Element::Var:
			return "x" + to_string(node.getVariableIndex());
		default: throw logic_error("Неизвестное состояние");
		}
	}
};

class Simplifier {
	Calculator calculator;
	vector<int> variables;
public:
	void simplify(unique_ptr<Node>& node) const {
		switch (node->getElement()) {
		case Element::Const:
		case Element::Var:
			return;
		case Element::Exp:
		case Element::Log:
		case Element::Neg:
		case Element::Conv:
		case Element::Sqrt: {
			simplify(node->left);
			if (node->getLeft()->getElement() != Element::Const)
				return ;
			node = Node::makeConst(
				calculator.calculate(
					*node,
					variables
				)
			);
			return;
		}
		case Element::Add:
		{
			simplify(node->left);
			simplify(node->right);
			if (node->left->el == Element::Const){
				if (node->left->value == 0.0) 
					node = move(node->right);
				else if (node->right->el == Element::Const)
					node = Node::makeConst(
						node->left->value + node->right->value
					);
				}
			else if(node->right->el == Element::Const && node->right->value == 0.0)
				node = move(node->left);
			return;
		}
		//Позжу добавить обработку случаев ln, 1/x
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
};

class GeneratorTree {
private:
	mt19937 rand;
	const double rad = 100;//Позже проанализировать оптимальные константы
	size_t maxDepth;
	size_t countVariables;

public:
	GeneratorTree(
		size_t maxDepth,
		size_t countVariables
	) :rand(random_device{}()),
		maxDepth(maxDepth),
		countVariables(countVariables)
	{
		if (maxDepth < 1)
			throw logic_error("Максимальная глубина должна быть >=1");
		if (countVariables < 1)
			throw logic_error("Количество переменных должно быть >=1");
	}

	unique_ptr<Node> generateTerminal() {
		if (countVariables < 1)
			throw logic_error("Недопустимое количество переменных");
		uniform_int_distribution<> constOrVar(0, 1);
		if (constOrVar(rand)) {
			uniform_real_distribution<> constDist(-rad, rad);
			return Node::makeConst(constDist(rand));
		}
		uniform_int_distribution<> varDist(0, countVariables-1);
		return Node::makeVar(
			varDist(rand)
		);
	}

	unique_ptr<Node> generateUnarOper(size_t depth) {
		uniform_int_distribution<> unarOperDist(2, 6);
		return Node::makeUnarOper(
			static_cast<Element>(unarOperDist(rand)),
			generate(depth+1)
		);
	}

	unique_ptr<Node> generateBinOper(size_t depth) {
		uniform_int_distribution<> binOperDist(7, 8);
		return Node::makeBinOper(
			static_cast<Element>(binOperDist(rand)),
			generate(depth + 1),
			generate(depth + 1)
		);
	}
	//Позже проанализировать оптимальное распределение вероятностей категорий узлов
	//Оптимльная формула не должна иметь глубокие унарные узлы
	unique_ptr<Node> generate(size_t depth = 1) {
		if (depth < maxDepth) {
			uniform_int_distribution<> operDist(0, 9);
			switch (operDist(rand)) {
			case 0:
			case 1:
			case 2:
			case 3:
				return generateTerminal();//40% выпадения терминального узла
			case 4:
			case 5:
				return generateUnarOper(depth);//20% выпадения унарного узла 
			case 6:
			case 7:
			case 8:
			case 9:
				return generateBinOper(depth);//40% выпадения бинарного узла
			}
		}
		return generateTerminal();//Проверить логику глубины
	};
};

struct Data {
	vector<int> parameters;
	double answer;
};

class Meter {
private:
	vector<Data> data;
	double fine = 0.0;
	Calculator calculator;
public:
	Meter() {
		ifstream inFile("D:/Program Files/COURSE/dataTEST");
		if (!inFile.is_open()) {
			cerr << "Не удалось открыть файл";
		}
		else {
			
		}
	}
	double measure(Node& node) {
		double mse;
		for (int i = 0; i < data.size(); i++)
			mse += pow(calculator.calculate(node, data[i].parameters) - data[i].answer, 2);
		return mse / data.size() + fine * (node.size());
	}
};

struct UsingNode {
	unique_ptr<Node>* node;
	size_t depth;
};

class Improver {
private:
	mt19937 rand;
	size_t maxDepth;
	size_t count;
	GeneratorTree generator;
public:
	Improver(
		size_t maxDepth,
		size_t count,
		size_t countVariables
	) :rand(random_device{}()),
		maxDepth(maxDepth),
		count(count),
		generator(maxDepth,countVariables)
	{
		if (maxDepth < 1)
			throw logic_error("Максимальная глубина должна быть >=1");
		if (count < 1)
			throw logic_error("Количество родителей должно быть >=1");
	}

	size_t selection(
		const vector<double>& measures
	) {
		uniform_int_distribution<> idParent(0, measures.size()-1);
		size_t res = 0; double measure = +INFINITY;
		for (int i = 0; i < count; i++) {
			size_t id = idParent(rand);
			if (measure > measures[id]) {
				measure = measures[id];
				res = id;
			}
		}
		return res;
	}

	void usingNodes(
		vector<UsingNode>& nodes,
		unique_ptr<Node>& node,
		size_t depth
	) {
		if (!node)
			return;
		nodes.push_back({ &node,depth });
		usingNodes(nodes, node->left, depth + 1);
		usingNodes(nodes, node->right, depth + 1);
	}

	unique_ptr<Node> mutateFunction(const Node& parent) {
		unique_ptr<Node> son = parent.clone();
		vector<UsingNode> nodes;
		usingNodes(nodes, son, 1);
		vector<UsingNode> currentNodes;
		for (UsingNode& node : nodes) 
			if (Node::arity(node.node->get()->el) > 0)
				currentNodes.push_back(node);
		if (currentNodes.empty())
			return son;
		uniform_int_distribution<> nodeDist(0, currentNodes.size() - 1);
		UsingNode node = currentNodes[nodeDist(rand)];
		Element nodeEl = node.node->get()->el;
		Element el;
		if (Node::arity(nodeEl) == 1) {
			uniform_int_distribution<> unarOperDist(2, 6);
			do {
				el = static_cast<Element>(unarOperDist(rand));
			} while (nodeEl == el);
		}
		else {
			uniform_int_distribution<> binOperDist(7, 8);
			do {
				el = static_cast<Element>(binOperDist(rand));
			} while (nodeEl == el);
		}
		node.node->get()->el = el;
		return son;
	}

	unique_ptr<Node> mutateSubTree(const Node& parent) {
		unique_ptr<Node> son = parent.clone();
		vector<UsingNode> nodes;
		usingNodes(nodes, son, 1);
		uniform_int_distribution<> nodeDist(0, nodes.size() - 1);
		UsingNode node = nodes[nodeDist(rand)];
		*node.node = move(generator.generate(node.depth));
		return son;
	}

	void mutate() {

	}

};

int main()
{
	/*unique_ptr<Node> x1 = Node::makeVar(0);
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
	int n, m;
	cin >> n >> m;
	vector<int> variables{ n, m };
	cout << calculator.getFormula(*mult) << endl;
	cout << calculator.calculate(*mult, variables);
	cout << calculator.getOptimizeFormula(*mult);*/

	Calculator calculator;
	int depth; cin >> depth;
	int n; cin >> n;
	GeneratorTree generator(depth, n);
	for (int i = 0; i < 10; i++) {
		unique_ptr<Node> node = generator.generate();
		cout << calculator.getFormula(*node) << endl;
	}

}