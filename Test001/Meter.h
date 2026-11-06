#pragma once
#include "Data.h"
#include "Calculator.h"
#include <stdexcept>
#include <fstream>
#include <iostream>


class Meter {
private:
	vector<Data> data;
	double fine = 0.0;
	Calculator calculator;

	//Глобальные переменные
	size_t maxDepth;//Максимальная глубина дерева выражения
	size_t countVariables;//Число переменных функции
	size_t sizePopulation;//Размер каждой популяции
	size_t maxCountPopulation;//Максимальное число популяций
	size_t elitismCount;//Количество родителей переносимых в следующее поколение
	size_t sizeSelection;//Размер выборки при мутации деревьев
	double target;//Максимальное отклонение значений

public:

	Meter();

	size_t getMaxDepth() const;
	size_t getCountVariables() const;
	size_t getSizePopulation() const;
	size_t getmaxCountPopulation() const;
	size_t getelitismCount() const;
	double getTarget() const;

	double measure(Node& node);

};