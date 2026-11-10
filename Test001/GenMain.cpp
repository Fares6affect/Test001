#include "GenMain.h"
#include <future>
#include <iostream>

GenMain::GenMain()
{
	ifstream inFileSetting("SETTING_TEST.txt");
	if (!inFileSetting.is_open()) {
		throw runtime_error("Файл не найден");
	}
	
	string number;
	getline(inFileSetting, number);
	maxDepth = stoull(number);
	if (maxDepth < 1) throw logic_error("Максимальная глубина должна быть >= 1");
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
	target = stold(number);
	if (target < 0) throw logic_error("Числовая характеристика отклонения должна быть >= 0");
	getline(inFileSetting, number);
	fine = stold(number);
	if (fine < 0) throw logic_error("Штраф за размер должнен быть >= 0");
	inFileSetting.close();

	ifstream inFileData("DATA_TEST.txt");
	if (!inFileData.is_open()) {
		throw runtime_error("Файл не найден");
	}
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
	inFileData.close();
	if (data.empty())
		throw runtime_error("Отсутствуют данные о функции");

	meter = make_unique<Meter>(data,fine);
};

size_t GenMain::getBestIsland(const vector<unique_ptr<Island>>& islands) const{
	size_t idBest = 0;
	double measureBest = islands[0].get()->getBestMeasure();
	for (size_t i = 1; i < islands.size(); i++) {
		double measure = islands[i].get()->getBestMeasure();
		if (measure < measureBest) {
			measureBest = measure;
			idBest = i;
		}
	}
	return idBest;
}

void GenMain::migration(vector<unique_ptr<Island>>& islands) {
	if (countIslands == 1)
		return;
	vector<vector<unique_ptr<Node>>> migrants;
	migrants.reserve(countIslands);
	for (const unique_ptr<Island>& island : islands)
		migrants.push_back(island.get()->getBestNodes(countMigrant));
	for (size_t i = 0; i < countIslands; i++)
		islands[(i + 1) % countIslands].get()->migrations(move(migrants[i]));
}

unique_ptr<Node> GenMain::getFunction() {
	vector<unique_ptr<Island>> islands;
	islands.reserve(countIslands);
	for (size_t i = 0; i < countIslands; i++) {
		islands.push_back(make_unique<Island>(
			sizePopulation,
			sizeSelection,
			elitismCount,
			maxDepth,
			countVariables,
			*meter
		));
		islands.back().get()->createPopulation();
	}
	size_t idBestIsland = getBestIsland(islands);
	for (size_t i = 1; i < maxCountPopulation; i++) {
		cout << i << endl;
		vector<future<void>> tasks;
		tasks.reserve(countIslands);

		for (const auto& island : islands) {
			Island* ptr = island.get();

			tasks.push_back(async(launch::async, [ptr]() {
				ptr->nextPopulation();
				}));
		}

		for (auto& task : tasks)
			task.get();
		if (i % intervalMigration == 0)
			migration(islands);
		idBestIsland = getBestIsland(islands);
		double bestMeasure = islands[idBestIsland].get()->getBestMeasure();
		cout << bestMeasure << endl;
		if (bestMeasure <= target)
			break;
	}
	return islands[idBestIsland].get()->getBestNode();
}