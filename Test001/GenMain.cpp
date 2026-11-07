#include "GenMain.h"
#include <iostream>

GenMain::GenMain()
{
	ifstream inFileSetting("D:/Program Files/COURSE/SETTING_TEST.txt");
	if (!inFileSetting.is_open()) {
		throw runtime_error("Файл не найден");
	}
	else {
		string number;
		getline(inFileSetting, number);
		sizePopulation = stoull(number);
		if (sizePopulation < 1) throw logic_error("Размер популяции должен быть >= 1");
		getline(inFileSetting, number);
		maxCountPopulation = stoull(number);
		if (maxCountPopulation < 1) throw logic_error("Число популяций должно быть >= 1");
		getline(inFileSetting, number);
		sizeSelection = stoull(number);;
		if (sizeSelection < 1) throw logic_error("Размер выборки должен быть >= 1");
		if (sizeSelection > sizePopulation) throw logic_error("Размер выборки должен быть <= размера популяции");
		getline(inFileSetting, number);
		elitismCount = stoull(number);
		if (elitismCount > sizePopulation) throw logic_error("Количество сохраняемых родителей должно быть <= размера популяции");
		getline(inFileSetting, number);
		maxDepth = stoull(number);
		if (maxDepth < 1) throw logic_error("Максимальная глубина должна быть >= 1");
		getline(inFileSetting, number);
		target = stold(number);
		if (target < 0) throw logic_error("Числовая характеристика отклонения должна быть >= 0");
		getline(inFileSetting, number);
		fine = stold(number);
		if (fine < 0) throw logic_error("Штраф за размер должнен быть >= 0");
		inFileSetting.close();

		ifstream inFileData("D:/Program Files/COURSE/DATA_TEST.txt");
		getline(inFileData, number);
		countVariables = stoull(number);
		if (countVariables < 1) throw logic_error("Число переменных должно быть >= 1");
		while (getline(inFileData, number)) {
			stringstream ss(number);
			Data d;
			int param;
			for (size_t i = 0; i < countVariables; i++) {
				if (!(ss >> param))
					throw runtime_error("Недостаточно аргументов");
				d.parameters.push_back(param);
			}
			if (!(ss >> d.answer))
				throw runtime_error("Отсутсвует значение функции");
			data.push_back(d);
		}
		if (data.empty())
			throw runtime_error("Отсутствуют данные о функции");
	}

	meter = make_unique<Meter>(data,fine);

	generator = make_unique<Generator>(
		maxDepth,
		countVariables
	);

	improver = make_unique<Improver>(
		maxDepth,
		sizeSelection,
		countVariables,
		*generator
	);

	simplifier = Simplifier();
};

Population GenMain::createPopulation() {
	Population population(sizePopulation, *meter);
	for (size_t i = 0; i < sizePopulation; i++) {
		unique_ptr<Node> node = generator.get()->generate();
		simplifier.simplify(node);
		population.addIndividual(move(node));
	}
	return population;
}

unique_ptr<Node> GenMain::getFunction() {
	Population oldPopulation = createPopulation();
	size_t idPopulation = 1;
	Population newPopulation = Population(sizePopulation, *meter);
	while (oldPopulation.getMeasureBest() > target && idPopulation < maxCountPopulation) {
		cout << oldPopulation.getMeasureBest()<<endl;
		vector<bool> selectedNodes(sizePopulation, false);
		const vector<double>& measures = oldPopulation.getMeasures();
		for (size_t i = 0; i < elitismCount; i++) {
			size_t idBestNode = sizePopulation;
			double measureBestNode = +INFINITY;
			for(size_t j=0;j<sizePopulation;j++)
				if (!selectedNodes[j] && measures[j] < measureBestNode) {
					measureBestNode = measures[j];
					idBestNode = j;
				}
			selectedNodes[idBestNode] = true;
			newPopulation.addIndividual(oldPopulation.getIndividual(idBestNode).node.get()->clone());
		}
		while (newPopulation.getSize() < sizePopulation) {
			size_t parent1 = improver.get()->selection(measures);
			size_t parent2 = improver.get()->selection(measures);
			unique_ptr<Node> son = improver.get()->improve(
				*oldPopulation.getIndividual(parent1).node,
				*oldPopulation.getIndividual(parent2).node
			);
			simplifier.simplify(son);
			newPopulation.addIndividual(move(son));
		}
		oldPopulation = move(newPopulation);
		newPopulation.clear();
		idPopulation++;
		cout << idPopulation<<endl;
	}
	return oldPopulation.getIndividual(oldPopulation.getIdBest()).node.get()->clone();
}