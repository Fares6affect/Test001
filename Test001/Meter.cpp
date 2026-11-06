#include "Meter.h"

Meter::Meter() {
	ifstream inFile("D:/Program Files/COURSE/dataTEST");
	if (!inFile.is_open()) {
		cerr << "Не удалось открыть файл";
	}
	else {

	}
}
double Meter::measure(Node& node) {
	double mse = 0.0;
	for (int i = 0; i < data.size(); i++)
		mse += pow(calculator.calculate(node, data[i].parameters) - data[i].answer, 2);
	return mse / data.size() + fine * (node.size());
}