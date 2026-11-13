#include "GeneratorFunction.h"


GeneratorFunction::GeneratorFunction() {

	ifstream inFileSetting("SETTING_TEST.txt");
	if (!inFileSetting.is_open()) {
		throw runtime_error("Файл не найден");
	}
	ofstream outFileData("DATA_TEST.txt");
	if (!outFileData.is_open()) {
		throw runtime_error("Файл не найден");
	}

	string number;
	getline(inFileSetting, number);
	size_t maxDepth = stoull(number);
	if (maxDepth < 1) throw logic_error("Максимальная глубина должна быть >= 1");
	inFileSetting.close();

	mt19937 rand;
	uniform_int_distribution<> countVarDist(0, 6);
	size_t countVariables = countVarDist(rand);
	countVariables = 1;
	outFileData << countVariables << endl;

	Generator generator(maxDepth, countVariables);
	Simplifier simplifier;
	Calculator calculator;

	unique_ptr<Node> function = generator.generate();
	while (function.get()->el == Element::Const)
		function = generator.generate();
	simplifier.simplify(function);
	cout << calculator.getFormula(*function)<<endl;

	uniform_int_distribution<> countDataDist(5, 20);
	size_t countData = countDataDist(rand);
	uniform_int_distribution<> varDist(-100, 100);
	for (size_t i = 0; i < countData; i++) {
		vector<int> data;
		for (size_t j = 0; j < countVariables; j++) {
			data.push_back(varDist(rand));
			outFileData << data.back() << " ";
		}
		outFileData << calculator.calculate(*function,data) << endl;
	}

	outFileData.close();
}