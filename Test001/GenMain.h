#pragma once
#include "Generator.h"
#include "Improver.h"
#include "Meter.h"
#include "Simplifier.h"
#include "Population.h"
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

	vector<Data> data;//Данные функции
	double fine = 0.0;//Штраф за размер выражения

	unique_ptr<Meter> meter;
	unique_ptr<Generator> generator;
	unique_ptr<Improver> improver;
	Simplifier simplifier;

	Population createPopulation();

public:
	GenMain();

	unique_ptr<Node> islandEvolve(
		Population& population
	);

	unique_ptr<Node> getFunction();
};