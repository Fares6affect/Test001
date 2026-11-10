#include "Generator.h"

Generator::Generator(
	size_t maxDepth,
	size_t countVariables
) :rand(random_device{}()),
maxDepth(maxDepth),
countVariables(countVariables){ }

unique_ptr<Node> Generator::generateTerminal() {
	if (countVariables < 1)
		throw logic_error("Недопустимое количество переменных");
	uniform_int_distribution<> constOrVar(0, 1);
	if (constOrVar(rand)) {
		uniform_real_distribution<> exponentDist(-6.0, 6.0);
		uniform_real_distribution<> signDist(0,1);
		return Node::makeConst((signDist(rand)*2-1)*pow(10.0,exponentDist(rand)));
	}
	uniform_int_distribution<> varDist(0, countVariables - 1);
	return Node::makeVar(
		varDist(rand)
	);
}

unique_ptr<Node> Generator::generateUnarOper(size_t depth) {
	uniform_int_distribution<> unarOperDist(2, 7);
	return Node::makeUnarOper(
		static_cast<Element>(unarOperDist(rand)),
		generate(depth + 1)
	);
}

unique_ptr<Node> Generator::generateBinOper(size_t depth) {
	uniform_int_distribution<> binOperDist(8, 9);
	return Node::makeBinOper(
		static_cast<Element>(binOperDist(rand)),
		generate(depth + 1),
		generate(depth + 1)
	);
}
//Позже проанализировать оптимальное распределение вероятностей категорий узлов
//Оптимльная формула не должна иметь глубокие унарные узлы
unique_ptr<Node> Generator::generate(size_t depth) {
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