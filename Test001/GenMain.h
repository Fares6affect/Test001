#pragma once
#include "Meter.h"
#include "Island.h"
#include <utility>
#include <fstream>
#include <sstream>

class GenMain {
private:

	//Глобальные переменные
	size_t sizePopulation;//Размер каждой популяции
	size_t maxCountPopulation;//Максимальное число популяций
	size_t sizeSelection;//Размер выборки при мутации деревьев
	size_t elitismCount;//Количество родителей переносимых в следующее поколение

	size_t maxDepth;//Максимальная глубина дерева выражения
	size_t countVariables;//Число переменных функции
	double target;//Максимальное отклонение значений

	size_t countIslands=4;//Число островов
	size_t intervalMigration=10;//Интервал миграции
	size_t countMigrant=5;//Число мигрантов

	vector<Data> data;//Данные функции
	double fine = 0.0;//Штраф за размер выражения

	unique_ptr<Meter> meter;

	Population createPopulation();

public:
	GenMain();

	unique_ptr<Node> islandEvolve(
		Population& population
	);

	size_t getBestIsland(const vector<unique_ptr<Island>>& islands) const;

	void migration(vector<unique_ptr<Island>>& islands);

	unique_ptr<Node> getFunction();
};