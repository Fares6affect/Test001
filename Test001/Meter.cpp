#include "Meter.h"

Meter::Meter() {
	ifstream inFile("D:/Program Files/COURSE/dataTEST");
	if (!inFile.is_open()) {
		cerr << "Не удалось открыть файл";
	}
	else {

	}
	size_t maxDepth=0;
	size_t countVariables=0;
	if (maxDepth < 1)
		throw logic_error("Максимальная глубина должна быть >=1");
	if (countVariables < 1)
		throw logic_error("Количество переменных должно быть >=1");
}

//Геттеры глобальных переменных
size_t Meter::getMaxDepth() const{
	return maxDepth;
}
size_t Meter::getCountVariables() const{
	return countVariables;
}
size_t Meter::getSizePopulation() const {
	return sizePopulation;
}
size_t Meter::getmaxCountPopulation() const {
	return maxCountPopulation;
}
size_t Meter::getelitismCount() const {
	return elitismCount;
}
double Meter::getTarget() const {
	return target;
}

double Meter::measure(Node& node) {
	double mse = 0.0;
	for (int i = 0; i < data.size(); i++)
		mse += pow(calculator.calculate(node, data[i].parameters) - data[i].answer, 2);
	return mse / data.size() + fine * (node.size());
}