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
	outFileData.close();

	string number;
	getline(inFileSetting, number);
	size_t maxDepth = stoull(number);
	if (maxDepth < 1) throw logic_error("Максимальная глубина должна быть >= 1");
	inFileSetting.close();

	mt19937 rand;
	uniform_int_distribution<> countVarDist(1, 3);
	size_t countVariables = countVarDist(rand);

	Generator generator(
		maxDepth, 
		countVariables, 
		IslandStrategy::Local
	);
	Simplifier simplifier;
	Calculator calculator;

	bool f;
	do {
		f = false;
		outFileData.open("DATA_TEST.txt", ios::out | ios::trunc);
		outFileData << countVariables << endl;

		unique_ptr<Node> function = generator.generate();
		while (function.get()->el == Element::Const)
			function = generator.generate();
		simplifier.simplify(function);
		cout << calculator.getFormula(*function) << endl;

		uniform_int_distribution<> countDataDist(5, 20);
		size_t countData = countDataDist(rand);
		uniform_int_distribution<> varDist(-100, 100);
		for (size_t i = 0; i < countData; i++) {
			vector<int> data;
			for (size_t j = 0; j < countVariables; j++) {
				data.push_back(varDist(rand));
				outFileData << data.back() << " ";
			}
			CalcResult cr = calculator.tryCalculate(*function, data);
			if (!cr.isValid()) {
				f = true;
				cout<<"ERROR"<<endl<<endl;
				break;
			}
			outFileData << cr.value << endl;
		}
		outFileData.close();
	} while (f);

	outFileData.close();
}